// Fill out your copyright notice in the Description page of Project Settings.

#include "BTService_CheckForPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "MainCharacter.h"
#include "ComponentPawn.h"
#include "MyAIController.h"


//**Contructor
UBTService_CheckForPlayer::UBTService_CheckForPlayer()
{
	//**Set to create as an instance
	bCreateNodeInstance = true; 
}

//**Tick over ride function
void UBTService_CheckForPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	//**Get the enemy AI's player controller
	AMyAIController *EnemyPC = Cast<AMyAIController>(OwnerComp.GetAIOwner());

	//**if the Enemy player controller exists
	if (EnemyPC)
	{
		//**Get the first player pawn
		
		AMainCharacter *Player = Cast<AMainCharacter>(GetWorld()->GetFirstPlayerController()->GetPawn()); 
		//APawn *Player = GetWorld()->GetFirstPlayerController()->GetPawn(); 

		//**If the pawn exists
		if (Player)
		{
			//**Set the value of the black board componenet's Target to the player pawn
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(EnemyPC->EnemyKeyID, Player); 

			//**Debug message
			//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red, TEXT("PLAYER DETECTED")); 
		}

	}
}
