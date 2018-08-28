// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveFromPlayer.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyCharacter.h"
#include "MainCharacter.h"
#include "MyAIController.h"



EBTNodeResult::Type UBTTask_MoveFromPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//**Get the player controller for the owner of this AI
	AMyAIController *EnemyPC = Cast<AMyAIController>(OwnerComp.GetAIOwner()); 

	//**From the Owner of this component, get the BlackBoardComponent, then get the Value that is stored in the EnemyKeyID of the player controller we just got,
	//** which should return the value of the "Target" key that was set in the black board, which in turn should be the player character
	AMainCharacter *PlayerChar = Cast<AMainCharacter>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(EnemyPC->EnemyKeyID)); 

	//**If the cast worked
	if (PlayerChar)
	{
			
		//**Get pointer to the Enemey Character
		ACharacter *EnChar = Cast<ACharacter>(EnemyPC->GetCharacter());
		
		//**If the enemy character is less than 1000 distance to the player
		if (EnChar->GetDistanceTo(PlayerChar) < 1000.0f)
		{
			//**Cast to EnemyCharacter class and set the Chasing toggle to true
			AEnemyCharacter *enc = Cast<AEnemyCharacter>(EnChar);
			enc->Chasing = true; 

			//**Move to the player and show attack message
			EnemyPC->MoveToActor(PlayerChar, -1.0f, false, true, true, 0, true);
			//GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, "Attack!");			
		}
		
		//**Else stop moving
		else
		{
			//**Cast to EnemyCharacter class
			AEnemyCharacter *enc = Cast<AEnemyCharacter>(EnChar);

			//**If the character is chasing the player
			if (enc->Chasing)
			{
				//**Set the toggle to false, and stop movement
				enc->Chasing = false;
				EnChar->GetCharacterMovement()->StopMovementImmediately();
			}			
		}

		//**Succeeded
		return EBTNodeResult::Succeeded;
	}

	//**Failed
	else
	{
		return EBTNodeResult::Failed;
	}
}
