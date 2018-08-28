// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "Runtime/CoreUObject//Public/UObject/ConstructorHelpers.h"
#include "GameFramework/Pawn.h"
#include "MyPawnMovementComponent.h"
#include "CoyGAM312GameModeBase.h"
#include "EnemyCharacter.h"
#include "ComponentPawn.generated.h"

UCLASS()
class COYGAM312_API AComponentPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AComponentPawn();
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//**Override fuction to return the custom movement component created for this class. 
	virtual UPawnMovementComponent* GetMovementComponent() const override;

	//**Variable to track the MyPawnMovementComponent
	class UMyPawnMovementComponent* MovementComponent; 

	//**Movement functions
	void MoveForward(float AxisValue); 
	void MoveRight(float AxisValue); 
	void TurnRight(float AxisValue);
	void TurnUp(float AxisValue); 

	//**set speed
	void SetSpeed(float Speed); 

	//**On  hit
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

private:
	//**Pointer for a Spring Arm component to be used to attach the camera to the player
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	//**Pointer for the camera component to be attached to the player
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FixedCamera;

	//**Pointer to the game mode for score and UI messages
	ACoyGAM312GameModeBase *GameMode;
};
