// Fill out your copyright notice in the Description page of Project Settings.

#include "MyHUD.h"
#include "Engine.h"
#include "Engine/Canvas.h"
#include "CanvasItem.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"


//**Contructor
AMyHUD::AMyHUD()
{
	//**Set cross hair texture
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTextureObj(TEXT("/Game/StarterContent/Textures/FirstPersonCrosshair")); 
	CrosshairTexture = CrosshairTextureObj.Object;

	//**Set the health bar widget
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarObj(TEXT("/Game/UI/HealthUI"));
	HUDWidgetClass = HealthBarObj.Class; 
}

void AMyHUD::DrawHUD()
{
	Super::DrawHUD();
	
	//**Draw cross hairs
	//**Get the center of screen by getting the X & Y sizes of the cplipped draw canvas, and cutting in half
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	//**offset to compensate for the dimensions ofd the texture, so the texture is centered
	const FVector2D CrossHairDrawPos((Center.X), (Center.Y + 20.0f));

	//**Draw the cross hairs with translucent blend mode
	FCanvasTileItem TileItem(CrossHairDrawPos, CrosshairTexture->Resource, FLinearColor::White); 
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);

}

void AMyHUD::BeginPlay()
{
	Super::BeginPlay();

	//**If HUD Widget was succesfully loaded
	if (HUDWidgetClass)
	{
		//**Create the widget
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		
		//**If successful
		if (CurrentWidget)
		{
			//**Add widget to screen
			CurrentWidget->AddToViewport();
		}
	}

}
