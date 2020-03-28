// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DwarfBaseCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "engine/World.h"
#include "BehaviorTree/BehaviorTree.h"
#include "DwarfEnemyCharacter.generated.h"

/**
 * @brief All the logic that applies to enemies is here.
 */
UCLASS()
class GLADIATORGAME_API ADwarfEnemyCharacter : public ADwarfBaseCharacter
{
    GENERATED_BODY()

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;
    // Called every frame
    virtual void Tick(const float DeltaTime) override;

    // Implements the Character TakeDamage function
    virtual float TakeDamage(const float        DamageAmount, struct FDamageEvent const& DamageEvent,
                             class AController* EventInstigator,
                             AActor*            DamageCauser) override;

public:
    // Sets input
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
    // Permanently looks at the player
    void LookAt();
    // Event triggered when the Enemy dies
    void OnDeath() const;

    // The target Pawn for the lookAt
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
    APawn* Target;

    // The Behavior for the enemy
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
    UBehaviorTree* EnemyBT;

    // The type of enemy we want.
    UPROPERTY(EditDefaultsOnly, Category = "AI")
    uint8 BotType;
};
