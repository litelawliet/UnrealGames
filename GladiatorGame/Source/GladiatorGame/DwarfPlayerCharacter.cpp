// Fill out your copyright notice in the Description page of Project Settings.


#include "DwarfPlayerCharacter.h"
#include "GameFramework/Controller.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/InputComponent.h"
#include "Engine/World.h"
#include "TimerManager.h"

ADwarfPlayerCharacter::ADwarfPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;

	Camera = CreateDefaultSubobject <UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	// bUseControllerRotation = false;
}

void ADwarfPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	SetupHUD();
}

void ADwarfPlayerCharacter::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (StaminaTimeline != nullptr)
		StaminaTimeline->TickComponent(DeltaTime, ELevelTick::LEVELTICK_TimeOnly, nullptr);

	if (!bIsDead && HealthPercentage <= 0.0f)
	{
		bIsDead = true;
		PlayerDeathEvent.Broadcast();
	}
}

float ADwarfPlayerCharacter::TakeDamage(const float DamageAmount, FDamageEvent const& DamageEvent,
	AController* EventInstigator, AActor* DamageCauser)
{
	bCanBeDamaged = false;
	redFlash = true;
	UpdateHealth(-DamageAmount);
	DamageTimer();
	return DamageAmount;
}

void ADwarfPlayerCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ADwarfPlayerCharacter::OnFire);

	PlayerInputComponent->BindAxis("MoveX", this, &ADwarfPlayerCharacter::MovingX);
	PlayerInputComponent->BindAxis("MoveY", this, &ADwarfPlayerCharacter::MovingY);
	PlayerInputComponent->BindAxis("LookSides", this, &ADwarfPlayerCharacter::TurnCameraX);
	PlayerInputComponent->BindAxis("LookUp", this, &ADwarfPlayerCharacter::TurnCameraY);
	PlayerInputComponent->BindAxis("CameraZoom", this, &ADwarfPlayerCharacter::CameraZoom);
}

void ADwarfPlayerCharacter::MovingX(const float Value)
{
	if (!bIsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ADwarfPlayerCharacter::MovingY(const float Value)
{
	if (!bIsAttacking)
	{
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		const FVector Direction = FRotationMatrix(YawRotation).GetScaledAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void ADwarfPlayerCharacter::TurnCameraX(const float Value)
{
	AddControllerYawInput(Value * MouseAcceleration * GetWorld()->GetDeltaSeconds());
}

void ADwarfPlayerCharacter::TurnCameraY(const float Value)
{
	AddControllerPitchInput(-Value * MouseAcceleration * GetWorld()->GetDeltaSeconds());
}

void ADwarfPlayerCharacter::CameraZoom(const float Value)
{

    float TmpDist = CameraBoom->TargetArmLength;
    TmpDist += Value * ZoomIntensity;
    if (TmpDist > MinZoom && TmpDist < MaxZoom)
    {
        CameraBoom->TargetArmLength = TmpDist;
    }
}

void ADwarfPlayerCharacter::OnFire()
{
	if (!FMath::IsNearlyZero(Stamina, 0.001f) && bCanUseStamina && Stamina > StaminaCostPerAttack)
	{
		if (StaminaTimeline != nullptr)
			StaminaTimeline->Stop();
		GetWorldTimerManager().ClearTimer(StaminaTimerHandle);
		SetStaminaChange(-StaminaCostPerAttack);
		GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &ADwarfPlayerCharacter::UpdateStamina, StaminaFillingCooldown, false);
	}

	if (bIsAttacking == false && Stamina > StaminaCostPerAttack)
	{
        Attack();
	}
}

#pragma region HUD
void ADwarfPlayerCharacter::SetupHUD()
{
	bCanUseStamina = true;
	bCanBeDamaged = true;
	
	Health = FullHealth;
	HealthPercentage = 1.0f;

	Stamina = FullStamina;
	StaminaPercentage = 1.0f;
	PreviousStamina = StaminaPercentage;
	StaminaValue = 0.0f;
}

FText ADwarfPlayerCharacter::GetHealthIntText() const
{
	const int32 HP = FMath::RoundHalfFromZero(HealthPercentage * 100);

	const FString HPS = FString::FromInt(HP);
	const FString HealthHUD = HPS + FString(TEXT("%"));
	FText HPText = FText::FromString(HealthHUD);

	return HPText;
}

FText ADwarfPlayerCharacter::GetStaminaIntText() const
{
	const int32 SP = FMath::RoundHalfFromZero(StaminaPercentage * 100);

	const FString SPS = FString::FromInt(SP);
	const FString FullSPS = FString::FromInt(FullStamina);
	const FString StaminaHUD = SPS + FString(TEXT("/") + FullSPS);
	FText SPText = FText::FromString(StaminaHUD);

	return SPText;
}

void ADwarfPlayerCharacter::DamageTimer()
{
	GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ADwarfPlayerCharacter::SetDamageState, DamageTimeCooldown_RedFlash, false);
}

bool ADwarfPlayerCharacter::PlayFlash()
{
	if (redFlash)
	{
		redFlash = false;
		return true;
	}
	return false;
}
#pragma endregion