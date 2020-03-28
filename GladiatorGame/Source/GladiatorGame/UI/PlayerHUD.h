#pragma once

#include "GameFramework/HUD.h"
#include "PlayerHUD.generated.h"

/**
 * The player HUD, found on top left corner of the screen
 */
UCLASS()
class GLADIATORGAME_API APlayerHUD : public AHUD
{
	GENERATED_BODY()
public:
	APlayerHUD();

protected:
	virtual void BeginPlay() override;
	virtual void DrawHUD() override;


private:
    // The type of Widget we will use
	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

    // The current widget in use
	UPROPERTY(EditAnywhere, Category = "Health")
		class UUserWidget* CurrentWidget;

};
