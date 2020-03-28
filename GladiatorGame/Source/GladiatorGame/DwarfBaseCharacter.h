#pragma once

#include "GameFramework/Character.h"

#include "Components/BoxComponent.h"
#include "Components/TimelineComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Curves/CurveFloat.h"

#include "DwarfBaseCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeathDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDwarfHitDelegate);

// This class is used as a base for both the player and the enemies. Most of the behaviour are used in both characters.
UCLASS()
class GLADIATORGAME_API ADwarfBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ADwarfBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(const float DeltaTime) override;
	void ResetMaterialFXColor();

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // The Weapon's Mesh for the right hand. We use the Hammer here.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* RightWeapon;
    // The Weapon's Mesh for the right hand. We use the Shield here.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* LeftWeapon;

    // This is the Collider that is used to check for collisions when attacking.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UBoxComponent* EnemyBoxComponent;

#pragma region Attack

    // Starts the Attacking logic.
    UFUNCTION()
        void Attack();

    // Declares component overlap begin function
    UFUNCTION()
        void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                        UPrimitiveComponent* OtherComp,
                        int32                OtherBodyIndex,
                        bool                 bFromSweep,
                        const FHitResult&    SweepResult);


    // Declares component overlap end function
    UFUNCTION()
        void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    // This function is called when a successful hit on a Dwarf is registered. Bonk damage is the hammer's damage;
    UFUNCTION()
        void ApplyBonkDamage(AActor*     ActorToDamage, const float        DamageAmount, const FVector& HitDir, const
                             FHitResult& HitInfo, TSubclassOf<UDamageType> DamageType);

    // This function is responsible for the rate of attack of the dwarf. It reset values and conditions for an attack opportunity.
	UFUNCTION(BlueprintCallable)
		void RestoreAttackPossibility();

    // Should the dwarf receive damage or not right now.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
        bool bDealDamage = false;

    // This bool is set to true when mouse left button is pressed, if conditions for attacking were met.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Assets", meta = (AllowPrivateAccess = "true"))
		bool bIsAttacking = false;

    // Returns the hit model's color to original color.
    FTimerHandle FXHitReset;

    // Damage dealt with the Hammer
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
        float DamageToDwarves = 100.0f;

    // To know whether our attack successfully hit a Dwarf or not
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Settings")
        bool bIsAttackSuccess = false;

    /** We store information on whose collider we are hitting, so we can use that information on other spots.
    /* It is reset every when the collision zone is left.
    */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack Settings")
        UPrimitiveComponent* OtherCollider;

    // This information about the HitResult of the collision is stored for further usage.
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack Settings")
        FHitResult MyHit;

    //  We keep a reference for who we attacked last, as long as we're in collision. 
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Attack Settings")
        ADwarfBaseCharacter* LastHitDwarf;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack Settings")
		FTimerHandle AttackRate;

#pragma endregion


#pragma region Health
    // Value used as the variable for Health/Life
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		float Health = 1000.0f;

    // This is the maximum Health we can have.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float FullHealth = 1000.0f;

    // This is the amount of health the FullHealth refers to. It is used for the UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Health")
		float HealthPercentage = 1.0f;
#pragma endregion

#pragma region Stamina
    // This is the amount of Stamina the FullStamina refers to. It is used for the UI
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		float StaminaPercentage = 1.0f;

    // The last value for Stamina interpolation animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		float PreviousStamina = 1.0f;

    // The current value for Stamina interpolation animation.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		float StaminaValue = 0.0f;

    // The actual stamina that the player has. This is the one that is used on logic.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stamina")
		float Stamina = 100.0f;

    // How much stamina does an attack cost.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaCostPerAttack = 15.0f;

    // The maximum value the stamina can have.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float FullStamina = 100.0f;

    // How much time does it take to for stamina to regenerate.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float StaminaFillingCooldown = 5.0f;

    // Re-enable the player to attack after this delay.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		float RestoreAttackPossibilityCooldown = 0.8f;

    // The curve that is used for the stamina animation.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
		UCurveFloat* StaminaCurve;

    // Timeline used for the Stamina animation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
		UTimelineComponent* StaminaTimeline;

    // Delegate for the player death
    UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnDeathDelegate PlayerDeathEvent;

    // Delegate for the dwarf hit confirmation
    UPROPERTY(BlueprintAssignable, Category = "Events")
		FOnDwarfHitDelegate DwarfHitEvent;

    // Timer for the player screen hit FX
	FTimerHandle MemberTimerHandle;
    // Timer for the stamina
    FTimerHandle StaminaTimerHandle;

    // Value from the stamina curve
	float StaminaCurveFloatValue;
    // Value from the stamina timeline
	float StaminaTimelineValue;
    // Determines whether we can consume stamina or not.
    bool bCanUseStamina;

	UFUNCTION(BlueprintPure, Category = "Health")
		float GetHealth() const;

	UFUNCTION(BlueprintPure, Category = "Stamina")
		float GetStamina() const;

	UFUNCTION()
		void SetDamageState();

	UFUNCTION()
		void SetStaminaValue();

	UFUNCTION()
		void SetStaminaState();

	UFUNCTION()
		void SetStaminaChange(const float StaminaChange);

	UFUNCTION()
		void UpdateStamina();

	UFUNCTION(BlueprintCallable, Category = "Power")
		void UpdateHealth(const float HealthChange);
#pragma endregion 
};
