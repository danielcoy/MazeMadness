// Fill out your copyright notice in the Description page of Project Settings.

#include "CoyGAM312GameModeBase.h"
#include "MyHUD.h"
#include "MainCharacter.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"

//**Constructor
ACoyGAM312GameModeBase::ACoyGAM312GameModeBase()
	: Super()
{

	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bCanEverTick = true;

	//**Set HUD to the custom class
	HUDClass = AMyHUD::StaticClass();

	//**Set UI message trigger to false
	HasUIMessage = false;

	//**Initialze the score
	Score = 0.0f; 

	//**initialize enemy counter
	EnemyCount = 0; 

	//**Initialize star pick up counter
	StarCount = 0; 

	//**Set game won trigger to false
	GameWon = false; 
}

//**Begin play
void ACoyGAM312GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	//**Set the current state to playing
	SetCurrentState(EGamePlayState::EPlaying);


			
}

//**Tick
void ACoyGAM312GameModeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//**Pointer for the player character
	AMainCharacter *MyCharacter = Cast<AMainCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
		
	//**If able to get the player pawn
	if (MyCharacter)
	{
		//**If health is "near zero"
		if (FMath::IsNearlyZero(MyCharacter->GetHealth(), 0.001f))
		{
			//**Set state to Game over
			SetCurrentState(EGamePlayState::EGameOver);
		}				
	}

	//**If the star count is 0 AND the score is more than 0, then player wins. 
	//**Checking the score makes sure this isn't triggered before the stars are 
	//**populated and the count incremented accordingly.
	if (StarCount <= 0 && Score > 0.0f)
	{
		SetCurrentState(EGamePlayState::EWin);

	}
}

//**Get the state
EGamePlayState ACoyGAM312GameModeBase::GetCurrentState() const
{
	return CurrentState;
}

//**Set the state
void ACoyGAM312GameModeBase::SetCurrentState(EGamePlayState NewState)
{
	CurrentState = NewState;
	HandleNewState(CurrentState);
}

//**Hanlde state
void ACoyGAM312GameModeBase::HandleNewState(EGamePlayState NewState)
{
	//**Switch on game state
	switch (NewState)
	{
	case EGamePlayState::EPlaying:
		{
			//**actions while playing
			//**no actions for now
			break;
		}

	case EGamePlayState::EGameOver:
		{
			//**On Game Over state
			SetUIMessage("You have died");

			break;
		}
	case EGamePlayState::EWin:
		{
			SetUIMessage("You have found all of the stars!");
			GameWon = true;
			break; 
		}
	default:
	case EGamePlayState::EUnknown:
		{
			//**no actions for now
			break;
		}
	}
}

//**Get the UI message
FText ACoyGAM312GameModeBase::GetUIMessage()
{
	return UIMessage;
}

//**Set a UI Message
void ACoyGAM312GameModeBase::SetUIMessage(FString Message)
{
	UIMessage = FText::FromString(Message);
	//**Toggle message indicator
	HasUIMessage = true;
}

//**Trigger the UI message to play
bool ACoyGAM312GameModeBase::PlayUIMessage()
{
	if (HasUIMessage)
	{
		//**Toggle the HasUIMessage
		HasUIMessage = false;
		return true;
	}

	else
	{
		return false;
	}
}

//**Trigger the win message to play
bool ACoyGAM312GameModeBase::PlayWinMessage()
{
	if (GameWon)
	{
		//**Toggle the Win state
		GameWon = false;
		return true;
	}

	else
	{
		return false;
	}
}

//**Function to add a value to the score
void ACoyGAM312GameModeBase::AddToScore(float ScoreChange)
{
	Score += ScoreChange;
}

//**function to get the score value in FText format
FText ACoyGAM312GameModeBase::GetScore()
{
	//**convert score to int
	int32 ScoreInt = Score; 

	//**String to hold the message that will be sent to show the score
	FString ScoreMsg; 

	//**Counter for getting the number of leading zeroes to put before the score so that there are always a set number of digits displayed on screen
	int32 LeadingZeroCounter = 10000;

	//**while the score is less than the counter, add a leading zero
	//**Then divide by 10 and check again. This will cycle through the
	//** 10000s, 1000s, 100s, and 10s to see where the score falls
	while (ScoreInt < LeadingZeroCounter && LeadingZeroCounter > 1)
	{
		ScoreMsg += "0";

		LeadingZeroCounter /= 10;
	}

	//**Now add the score and return
	ScoreMsg += FString::FromInt(ScoreInt);

	return FText::FromString(ScoreMsg);

}

//**Get the current enemy count
int32 ACoyGAM312GameModeBase::GetEnemyCount()
{
	return EnemyCount;
}

//**Change the enemy count
void ACoyGAM312GameModeBase::ChangeEnemyCount(int32 ChangeAmount)
{
	EnemyCount += ChangeAmount;
}

//**get the current star count
int32 ACoyGAM312GameModeBase::GetStarCount()
{
	return StarCount;
}

//**change the star count
void ACoyGAM312GameModeBase::ChangeStarCount(int32 ChangeAmount)
{
	StarCount += ChangeAmount;
}


