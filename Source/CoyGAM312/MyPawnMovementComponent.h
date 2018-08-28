// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"
#include "CoreMinimal.h"
#include "GameFramework/PawnMovementComponent.h"
#include "MyPawnMovementComponent.generated.h"

/**
 * 
 */
UCLASS()
class COYGAM312_API UMyPawnMovementComponent : public UPawnMovementComponent
{
	GENERATED_BODY()
	
public:

	//**Constructor
	UMyPawnMovementComponent(); 
	//**Override the inherited TickComponent() function in order to handle the movement inputs
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	float MoveSpeed;
	
	
};
