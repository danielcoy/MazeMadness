// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuPlayerController.h"
#include "Blueprint/UserWidget.h"

//**Being play function
void AMainMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (wMainMenu)
	{
		MainMenuWidget = CreateWidget<UUserWidget>(this, wMainMenu);

		if (MainMenuWidget)
		{
			MainMenuWidget->AddToViewport();
		}

		bShowMouseCursor = true; 
	}
}




