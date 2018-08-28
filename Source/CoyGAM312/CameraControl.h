// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "GameFramework/Actor.h"
#include "CameraControl.generated.h"


UCLASS()
class COYGAM312_API ACameraControl : public AActor
{
	GENERATED_BODY()	
public:	
	// Sets default values for this actor's properties
	ACameraControl();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//**Actor object to hold pointer to Camera One
	UPROPERTY(EditAnywhere)
	AActor* ACameraOne;
	
	//**Actor object to hold pointer to Camera Two
	UPROPERTY(EditAnywhere)
	AActor* ACameraTwo;
	
	//**Actor object to hold pointer to the Fixe Cam
	UPROPERTY(EditAnywhere)
	AActor* AFixedCamera;
	
	//**Player Contoller pointer for getting a reference to the current player controller to set the target view
	APlayerController* KeyPlayerController; 

	//**Function for setting the view to camera one
	UFUNCTION()
	void CameraOne();

	//**Function for setting the view to camera two
	UFUNCTION()
	void CameraTwo();

	//**Function for setting the view to the fixed cam
	UFUNCTION()
	void FixedCamera();
		
};
