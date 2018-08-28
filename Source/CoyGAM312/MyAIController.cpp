// Fill out your copyright notice in the Description page of Project Settings.

#include "MyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyCharacter.h"


//**Constructor Method
AMyAIController::AMyAIController()
{
	//**Initialize black board component
	BlackBoardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackBoardComp")); 

	//**Initialize the behavior tree component
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorComp"));
}

//**Possess method
void AMyAIController::Possess(APawn *InPawn)
{
	Super::Possess(InPawn);

	//**Get the component pawn
	AEnemyCharacter *EnChar = Cast<AEnemyCharacter>(InPawn);

	//**If cast to Component Pawn successful, and the BotBehavior is loaded
	if(EnChar && EnChar->BotBehavior)
	{
		//**Initialize the Blackboard component
		BlackBoardComp->InitializeBlackboard(*EnChar->BotBehavior->BlackboardAsset);

		//**Get the black board's keyID
		EnemyKeyID = BlackBoardComp->GetKeyID("Target"); 

		//**Start the behavior tree
		BehaviorComp->StartTree(*EnChar->BotBehavior);
	}
}
