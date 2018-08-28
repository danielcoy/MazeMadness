// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject//Public/UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"
#include "HealthPack.generated.h"

UCLASS()
class COYGAM312_API AHealthPack : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHealthPack();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "HealthValue")
	float HealthValue; 

	UFUNCTION()
	float GetHealthValue();
	
	
};
