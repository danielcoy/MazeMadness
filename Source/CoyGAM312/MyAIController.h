// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "MyAIController.generated.h"

/**
 * 
 */
UCLASS()
class COYGAM312_API AMyAIController : public AAIController
{
	GENERATED_BODY()

	//**Black Board component for behavior
	UPROPERTY(transient)
	class UBlackboardComponent *BlackBoardComp;
	
	//**Behavior Tree Component
	UPROPERTY(transient)
	class UBehaviorTreeComponent *BehaviorComp;

public:
	//**Class contructor
	AMyAIController(); 

	//**Override the possess function 
	virtual void Possess(APawn *InPawn) override; 

	//**Key ID for the blackboard
	uint8 EnemyKeyID; 


	
	
};
