// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraControl.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ACameraControl::ACameraControl()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
}

// Called when the game starts or when spawned
void ACameraControl::BeginPlay()
{
	Super::BeginPlay();

	//**Get the player controller
	KeyPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	//**Set the Bindings for the change camer actions. Chaning to Camera One is binded to the '1' key, Camera two to the '2' key
	//** and the camera that is fixed behind the player character is binded to the '0' key
	UGameplayStatics::GetPlayerController(this, 0)->InputComponent->BindAction("CameraOne", IE_Pressed, this, &ACameraControl::CameraOne);
	UGameplayStatics::GetPlayerController(this, 0)->InputComponent->BindAction("CameraTwo", IE_Pressed, this, &ACameraControl::CameraTwo);
	UGameplayStatics::GetPlayerController(this, 0)->InputComponent->BindAction("FixedCamera", IE_Pressed, this, &ACameraControl::FixedCamera);
	

	//**Get the fixed camera
	AFixedCamera = KeyPlayerController->GetViewTarget();
	//**Set the fixed camera as the initial view
	KeyPlayerController->SetViewTarget(AFixedCamera); 

}

// Called every frame
void ACameraControl::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	

	
}

//**Set the view to Camera One
//**OVER HEAD MAP VIEW
void ACameraControl::CameraOne()
{
	//**Get a current reference to the player controller
	KeyPlayerController = UGameplayStatics::GetPlayerController(this, 0);
	
	//**If the player controller exists
	if (KeyPlayerController)
	{
		//**if the current view is the overhead cam, then change with blenc
		if (KeyPlayerController->GetViewTarget() == ACameraTwo)
		{
			KeyPlayerController->SetViewTargetWithBlend(ACameraOne, 1.0f);
		}

		//**else change with no blend
		else
		{
			//**Set the target view
			KeyPlayerController->SetViewTarget(ACameraOne);
		}
	}
}

//**Set the View to CameraTwo
//**OVER HEAD CAM VIEW
void ACameraControl::CameraTwo()
{
	//**Get a current reference to the player controller
	KeyPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	//**If the player controller exists
	if (KeyPlayerController)
	{
		//**If the current view is the overhead map, then change to this with blend
		if (KeyPlayerController->GetViewTarget() == ACameraOne)
		{
			//**Set the target view to blend to camera two over 1 second
			KeyPlayerController->SetViewTargetWithBlend(ACameraTwo, 1.0f);
		}

		//**Else, don't change with blend
		else
		{
			KeyPlayerController->SetViewTarget(ACameraTwo);
		}
	}
}

//**Set the View to CameraTwo
//**PLAYER VIEW
void ACameraControl::FixedCamera()
{
	//**Get a current reference to the player controller
	KeyPlayerController = UGameplayStatics::GetPlayerController(this, 0);

	//**If the player controller exists
	if (KeyPlayerController)
	{
		//**Set the target view to blend to the fixed cam over 1 second
		KeyPlayerController->SetViewTarget(AFixedCamera);
	}
}


