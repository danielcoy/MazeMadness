// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "CoyGAM312GameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject//Public/UObject/ConstructorHelpers.h"
#include "Components/SplineComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class COYGAM312_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	//**Behavior tree
	UPROPERTY(EditAnywhere, Category = behavior)
	class UBehaviorTree *BotBehavior; 
		
	//**Curve for the timeline. Originally intended to move along the spline, currently used to time the change in direction.
	UPROPERTY(EditAnywhere, Category = "Timeline")
	UCurveFloat *PathCurve; 

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//**bool value that toggles when the character is and isn't chasing the player
	bool Chasing;

	//**Function for hit
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);

	//**Particle System object to create explosion
	UParticleSystem *Explosion; 


private:

	//**Timeline for movement on spline path
	FTimeline PathTimeLine; 

	//**Vector for the movement direction. 
	FVector Movement;	

	//**Function to tick the timeline
	UFUNCTION()
	void TickTimeLine(float Value);

	//**Pointer to the game mode for score and UI messages
	ACoyGAM312GameModeBase *GameMode;
	
	virtual void BeginDestroy() override; 
};
