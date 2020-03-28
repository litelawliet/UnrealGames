// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DwarfBaseCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "DwarfPlayerCharacter.generated.h"

/*
 * @brief All logic that applies to the Player can be found here.
 */
UCLASS()
class GLADIATORGAME_API ADwarfPlayerCharacter : public ADwarfBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values
	ADwarfPlayerCharacter();
    // Sets values for the HUD
    void SetupHUD();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// Called every frame
	virtual void Tick(const float DeltaTime) override;

    // Logic when Player receives damage
	virtual float TakeDamage(const float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
    // Applies movement and rotation Horizontally depending on input
	void MovingX(const float Value);
    // Applies movement and rotation Vertically depending on input
	void MovingY(const float Value);
    // Applies Rotation to the camera Horizontally depending on input
	void TurnCameraX(const float Value);
    // Applies Rotation to the camera Vertically depending on input
	void TurnCameraY(const float Value);
    // Applies Zoom to the camera, if need be with the scrollwheel
    void CameraZoom(const float Value);

	// Attack function, uses mouse left button as default
	void OnFire();

    // Scalar for the mouse input
	UPROPERTY(EditAnywhere, Category = "Assets")
		float MouseAcceleration;

    // This component makes a collision test with everything except the enemies
    // so the camera never goes through walls
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		USpringArmComponent* CameraBoom;

    // The camera component, the eyes of the player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		UCameraComponent* Camera;

	// Base turn rate, in deg/sec. Other scaling may affect final turn rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseTurnRate;

	// Base look up/down rate, in deg/sec. Other scaling may affect final rate.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		float BaseLookUpRate;

    // Scalar for the mouse zoom
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
        float ZoomIntensity;

    // Min-Clamp for the camera zoom, the closest value we can have.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
        float MinZoom = 100.f;

    // Max-Clamp for the camera zoom, the farthest value we can have.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Assets", meta = (AllowPrivateAccess = "true"))
        float MaxZoom = 700.f;

#pragma region HUD
	// #### HEALTH FIELDS ####
#pragma region HealthFields
    // Should the red screen flash trigger
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
		bool redFlash = false;

    // How much until we should stop the red screen
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
		float DamageTimeCooldown_RedFlash = 2.0f;
#pragma endregion
	// #### STAMINA FIELDS ####
#pragma region StaminaFields

#pragma endregion
    // Text for the health
	UFUNCTION(BlueprintPure, Category = "Health")
		FText GetHealthIntText() const;

    // Text for the stamina
	UFUNCTION(BlueprintPure, Category = "Stamina")
		FText GetStaminaIntText() const;

    // Sets a timer for when damage is taken
	UFUNCTION()
		void DamageTimer();

    // Starts red screen animation
	UFUNCTION(BlueprintPure, Category = "Health")
		bool PlayFlash();

private:
    // To know whether the player diededed
	bool bIsDead = false;
#pragma endregion
};
