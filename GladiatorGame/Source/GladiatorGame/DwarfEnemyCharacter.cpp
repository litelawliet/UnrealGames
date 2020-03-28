// Fill out your copyright notice in the Description page of Project Settings.

#include "DwarfEnemyCharacter.h"
#include "engine/World.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"
#include "MyAIController.h"

void ADwarfEnemyCharacter::BeginPlay()
{
    Super::BeginPlay();
    Target = GetWorld()->GetFirstPlayerController()->GetPawn();
    // GetWorldTimerManager().SetTimer(StaminaTimerHandle, this, &ADwarfEnemyCharacter::Attack, 3, true);

    class AMyAIController* AIController = Cast<AMyAIController>(GetController());
    class ADwarfBaseCharacter* SensedPawn = Cast<ADwarfBaseCharacter>(Target);
    if (AIController)
    {
        AIController->SetTargetEnemy(SensedPawn);
    }

}

void ADwarfEnemyCharacter::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);

    LookAt();
    if (HealthPercentage <= 0)
        OnDeath();
}

float ADwarfEnemyCharacter::TakeDamage(const float DamageAmount,
    FDamageEvent const& DamageEvent, AController* EventInstigator,
    AActor* DamageCauser)
{
	bCanBeDamaged = false;
	UpdateHealth(-DamageAmount);
	return DamageAmount;
}

void ADwarfEnemyCharacter::SetupPlayerInputComponent(
    UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    PlayerInputComponent->BindAction("ROTATE_TEST", IE_Pressed, this, &ADwarfEnemyCharacter::LookAt);
}

void ADwarfEnemyCharacter::LookAt()
{
    if (HealthPercentage <= 0)
        return;

    const FVector TargetLocation = Target->GetActorLocation();
    const FVector PawnLocation = GetActorLocation();

    const FRotator BaseRotator = UKismetMathLibrary::FindLookAtRotation(PawnLocation, TargetLocation);
    SetActorRotation(BaseRotator);
}

void ADwarfEnemyCharacter::OnDeath() const
{
	if (EnemyBoxComponent)
		EnemyBoxComponent->DestroyComponent(false);

	class UCapsuleComponent* EnemyCapsuleComponent = GetCapsuleComponent();
	if (EnemyCapsuleComponent)
	{
		EnemyCapsuleComponent->DestroyComponent(false);
	}
}
