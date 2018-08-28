// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MyHUD.generated.h"

/**
 * 
 */
UCLASS()
class COYGAM312_API AMyHUD : public AHUD
{
	GENERATED_BODY()

public:
	AMyHUD();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HUDFont")
	UFont *HUDFont;

	virtual void DrawHUD() override;

	virtual void BeginPlay() override;

private:
	//**Cross hair
	class UTexture2D *CrosshairTexture; 

	UPROPERTY(EditAnywhere, Category = "Health")
	TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
	class UUserWidget *CurrentWidget;
	
	
};
