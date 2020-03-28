// Fill out your copyright notice in the Description page of Project Settings.

#include "DwarfBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "DwarfEnemyCharacter.h"

// Sets default values
ADwarfBaseCharacter::ADwarfBaseCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RightWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RightWeapon"));
	LeftWeapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("LeftWeapon"));
	EnemyBoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));

	RightWeapon->SetupAttachment(GetMesh());
	LeftWeapon->SetupAttachment(GetMesh());
	EnemyBoxComponent->SetupAttachment(GetMesh());

	const FVector NewPos = { EnemyBoxComponent->GetComponentLocation().X, EnemyBoxComponent->GetComponentLocation().Y + 80, EnemyBoxComponent->GetComponentLocation().Z + 60 };
	EnemyBoxComponent->SetRelativeLocation(NewPos);

	//defaultValues
	RestoreAttackPossibilityCooldown = 0.8f;
}

// Called when the game starts or when spawned
void ADwarfBaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	RightWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "WeaponPoint");
	LeftWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, "DualWeaponPoint");

	if (StaminaCurve)
	{
		FOnTimelineFloat TimelineCallback;
		FOnTimelineEventStatic TimelineFinishedCallback;

		TimelineCallback.BindUFunction(this, FName("SetStaminaValue"));
		TimelineFinishedCallback.BindUFunction(this, FName("SetStaminaState"));

		StaminaTimeline = NewObject<UTimelineComponent>(this, FName("Stamina Animation"));

		StaminaTimeline->AddInterpFloat(StaminaCurve, TimelineCallback);
		StaminaTimeline->SetTimelineFinishedFunc(TimelineFinishedCallback);
		StaminaTimeline->RegisterComponent();
	}
	EnemyBoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ADwarfBaseCharacter::OnOverlapBegin);
	EnemyBoxComponent->OnComponentEndOverlap.AddDynamic(this, &ADwarfBaseCharacter::OnOverlapEnd);
	EnemyBoxComponent->SetActive(false);
}

// Called every frame
void ADwarfBaseCharacter::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (OtherCollider != nullptr && bIsAttacking && bDealDamage)
    {
        class UCapsuleComponent* DwarfCollider = Cast<UCapsuleComponent>(OtherCollider);

        if (DwarfCollider == nullptr)
            return;

        bDealDamage = false;

        class ADwarfBaseCharacter* Dwarf = Cast<ADwarfBaseCharacter>(DwarfCollider->GetOwner());

        if (Dwarf == nullptr || Dwarf->GetClass() == this->GetClass())
            return;

        LastHitDwarf = Dwarf;
        Dwarf->ApplyBonkDamage(Dwarf, DamageToDwarves, Dwarf->GetActorLocation(), MyHit, nullptr);
        DwarfHitEvent.Broadcast();
        Dwarf->GetMesh()->SetVectorParameterValueOnMaterials("FXHit", FVector4{255, 0, 0, 0});
        GetWorldTimerManager().SetTimer(FXHitReset, this, &ADwarfEnemyCharacter::ResetMaterialFXColor, 0.5, false);
    }
}

void ADwarfBaseCharacter::ResetMaterialFXColor()
{
    if (LastHitDwarf == nullptr)
        return;

    LastHitDwarf->GetMesh()->SetVectorParameterValueOnMaterials("FXHit", FVector4{  1,1, 1, 0 });
    LastHitDwarf = nullptr;
}

// Called to bind functionality to input
void ADwarfBaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADwarfBaseCharacter::Attack()
{
    if (bIsAttacking == false && Stamina > StaminaCostPerAttack)
    {
        EnemyBoxComponent->SetActive(true);
        bIsAttacking = true;
        bDealDamage = true;
        GetWorldTimerManager().SetTimer(AttackRate, this, &ADwarfBaseCharacter::RestoreAttackPossibility, RestoreAttackPossibilityCooldown, false);
    }
}
void ADwarfBaseCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	class UBoxComponent* ThisBox = Cast<UBoxComponent>(OverlappedComp);
	if (ThisBox == nullptr)
		return;

	class UCapsuleComponent* Capsule = Cast<UCapsuleComponent>(OtherComp);
	if (Capsule == nullptr)
		return;
	
	bIsAttackSuccess = true;
	OtherCollider = OtherComp;
	MyHit = SweepResult;
}
void ADwarfBaseCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    bIsAttackSuccess = false;
    OtherCollider = nullptr;
	bIsAttackSuccess = false;
	//GetWorldTimerManager().ClearTimer(AttackRate);
} 

void ADwarfBaseCharacter::ApplyBonkDamage(AActor* ActorToDamage, const float DamageAmount, const FVector& HitDir, const FHitResult& HitInfo, TSubclassOf<UDamageType> DamageType)
{
	UGameplayStatics::ApplyPointDamage(ActorToDamage, DamageAmount, HitDir, HitInfo, nullptr, this, DamageType);
}

void ADwarfBaseCharacter::RestoreAttackPossibility()
{
	bIsAttacking = false;
	EnemyBoxComponent->SetActive(false);
}

float ADwarfBaseCharacter::GetHealth() const
{
	return HealthPercentage;
}

float ADwarfBaseCharacter::GetStamina() const
{
	return StaminaPercentage;
}

void ADwarfBaseCharacter::SetDamageState()
{
	bCanBeDamaged = true;
}

void ADwarfBaseCharacter::SetStaminaValue()
{
	if (StaminaTimeline != nullptr)
		StaminaTimelineValue = StaminaTimeline->GetPlaybackPosition();

	StaminaCurveFloatValue = PreviousStamina + StaminaValue * StaminaCurve->GetFloatValue(StaminaTimelineValue);
	Stamina = FMath::Clamp(StaminaCurveFloatValue * FullStamina, 0.0f, FullStamina);

	StaminaPercentage = FMath::Clamp(StaminaCurveFloatValue, 0.0f, 1.0f);
}

void ADwarfBaseCharacter::SetStaminaState()
{
	bCanUseStamina = true;
	StaminaValue = 0.0f;
}

void ADwarfBaseCharacter::SetStaminaChange(const float StaminaChange)
{
	bCanUseStamina = false;
	PreviousStamina = StaminaPercentage;
	StaminaValue = StaminaChange / FullStamina;

	if (StaminaTimeline != nullptr)
		StaminaTimeline->PlayFromStart();
}

void ADwarfBaseCharacter::UpdateStamina()
{
	PreviousStamina = StaminaPercentage;
	StaminaPercentage = Stamina / FullStamina;
	StaminaValue = 1.0f;

	if (StaminaTimeline != nullptr)
		StaminaTimeline->PlayFromStart();
}

void ADwarfBaseCharacter::UpdateHealth(const float HealthChange)
{
	Health = FMath::Clamp(Health += HealthChange, 0.0f, FullHealth);
	HealthPercentage = Health / FullHealth;
}