// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject//Public/UObject/ConstructorHelpers.h"
#include "CoyGAM312GameModeBase.h"
#include "StarPickUp.generated.h"

UCLASS()
class COYGAM312_API AStarPickUp : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStarPickUp();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//**Function for hit
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	//**Pointer to the game mode for score and UI messages
	ACoyGAM312GameModeBase *GameMode;

	//**Particle System object to create sparks
	UParticleSystem *Sparks;

	
	
};
