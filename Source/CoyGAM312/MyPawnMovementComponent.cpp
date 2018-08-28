// Fill out your copyright notice in the Description page of Project Settings.

#include "MyPawnMovementComponent.h"

//**constructor
UMyPawnMovementComponent::UMyPawnMovementComponent() : MoveSpeed(600.0f)
{

}

void UMyPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//**Check if everything is still valid, if not return
	if (!PawnOwner || !UpdatedComponent || ShouldSkipUpdate(DeltaTime))
	{
		return; 
	}

	// Get (and then clear) the movement vector that we set in AComponentPawn::Tick
	FVector DesiredMovementThisFrame = ConsumeInputVector().GetClampedToMaxSize(1.0f) * DeltaTime * MoveSpeed;
	if (!DesiredMovementThisFrame.IsNearlyZero())
	{
		FHitResult Hit;
		//**Move the pawn component while respecting solid objects and barriers
		SafeMoveUpdatedComponent(DesiredMovementThisFrame, UpdatedComponent->GetComponentRotation(), true, Hit);

		// If we bumped into something, try to slide along it
		if (Hit.IsValidBlockingHit())
		{
			//**Allows the pawn to slide along an object after a collision, instead of just stopping in place and sticking to the object
			SlideAlongSurface(DesiredMovementThisFrame, 1.f - Hit.Time, Hit.Normal, Hit);
					
			
			//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Blue, name);
			 
		}
	}
}

