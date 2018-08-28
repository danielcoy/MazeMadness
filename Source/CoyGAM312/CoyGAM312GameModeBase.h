// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CoyGAM312GameModeBase.generated.h"

/**
 * 
 */
UENUM()
enum class EGamePlayState
{
	EPlaying, 
	EGameOver,
	EWin,
	EUnknown

};

UCLASS()
class COYGAM312_API ACoyGAM312GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ACoyGAM312GameModeBase();

	//**Override the begin play funtion
	virtual void BeginPlay() override;

	//**Override tick function 
	virtual void Tick(float DeltaTime) override; 
		
	//**Return the current game state
	UFUNCTION(BlueprintPure, Category = "Health")
	EGamePlayState GetCurrentState() const;

	//**Set the current game state
	void SetCurrentState(EGamePlayState NewState);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIMessage")
	FText UIMessage; 

	//**game score
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIMessage")
	float Score;

	//**Trigger for creating a UI message
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIMessage")
	bool HasUIMessage;

	//**Trigger for winning
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UIMessage")
	bool GameWon;

	//**Funtion to get the current UI message
	UFUNCTION(BlueprintPure, Category = "UIMessage")
	FText GetUIMessage();

	//**Function to set the current UI message
	UFUNCTION()
	void SetUIMessage(FString Message);

	//**Play the UI message
	UFUNCTION(BlueprintPure, Category = "UIMessage")
	bool PlayUIMessage();

	//**Play the win message
	UFUNCTION(BlueprintPure, Category = "UIMessage")
	bool PlayWinMessage();

	//**Function to add to the score
	UFUNCTION()
	void AddToScore(float ScoreChage);

	//**Function to get score in text format
	UFUNCTION(BlueprintPure, Category = "UIMessage")
	FText GetScore();

	//**Get the enemy count
	UFUNCTION(BlueprintPure, Category = "UIMessage")
	int32 GetEnemyCount();

	//**Get the star count
	UFUNCTION(BlueprintPure, Category = "UIMessage")
	int32 GetStarCount();

	//**Change enemy count
	void ChangeEnemyCount(int32 ChangeAmount);

	//**Change star count
	void ChangeStarCount(int32 ChangeAmount);

private:

	EGamePlayState CurrentState;

	void HandleNewState(EGamePlayState NewState);

	//**Enemy counter
	int32 EnemyCount; 

	//**Star Pick Up Count
	int32 StarCount;
	
	
};
