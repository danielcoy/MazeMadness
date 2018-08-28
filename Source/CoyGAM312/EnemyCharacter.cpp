// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacter.h"
#include "ComponentPawn.h"



// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	//**Set the Chasing to false.
	Chasing = false; 

	//**Set movement speed
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	//**Create the static mesh component
	UStaticMeshComponent* SMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual"));
	//**Attached the mesh to the root component
	SMesh->SetupAttachment(RootComponent);

	//**Find the 3d model asset for the cube in the starter content folders
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CapsuleVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_NarrowCapsule.Shape_NarrowCapsule"));
	//**If the visual asset was successfully found, then set its parameters
	if (CapsuleVisualAsset.Succeeded())
	{
		//**Set the Static Mesh to the visual asset we just found
		SMesh->SetStaticMesh(CapsuleVisualAsset.Object);
		//**Set the relative location of the mesh
		SMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -80.0f));
		//**Set the scale size of the mesh
		SMesh->SetWorldScale3D(FVector(1.7f));
	}

	//**Set the material for the mesh
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("/Game/Materials/M_EnemyMat"));
	if (CubeMaterial.Succeeded())
	{		
		SMesh->SetMaterial(0, CubeMaterial.Object);
	}

	//**Set scale of the root component
	RootComponent->SetWorldScale3D(FVector(0.5f, 0.5f, 0.35f));

	//**Load the explosion particle emmitted asset
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ExpPS(TEXT("/Game/StarterContent/Particles/P_Explosion.P_Explosion"));

	//**If successfully found and loaded
	if (ExpPS.Succeeded())
	{
		//**Set the Explosion object to the the asset
		Explosion = ExpPS.Object; 
	}
		
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	//**Set the movement vector to a random X & Y direction to start
	Movement = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0);

	//**If the PathCurve exists. 
	if (PathCurve)
	{
		//**Set the function to be called for each tick of the timeline
		FOnTimelineFloat ProgressFuction;
		ProgressFuction.BindUFunction(this, FName("TickTimeLine")); 
		//**Set the tick function and the curve to use for the timeline
		PathTimeLine.AddInterpFloat(PathCurve, ProgressFuction);
		//**Set looping to true
		PathTimeLine.SetLooping(true); 
		//**Start the timeline
		PathTimeLine.PlayFromStart();
	}	

	//**Register the OnHit event
	TScriptDelegate<FWeakObjectPtr> onHit;
	onHit.BindUFunction(this, "OnHit");
	OnActorHit.Add(onHit);

	//**Cast the GameMode
	GameMode = Cast<ACoyGAM312GameModeBase>(GetWorld()->GetAuthGameMode());

	//**Add to enemy count
	GameMode->ChangeEnemyCount(1);
	
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//**Advance the timeline
	PathTimeLine.TickTimeline(DeltaTime); 

}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

//**Tick event for the timeline curve.
void AEnemyCharacter::TickTimeLine(float Value)
{
	//**Only add movement input if the character is not chasing the player. Movement is handled by the AIController when it is chasing.
	if (!Chasing)
	{
		//**Add the movement input
		AddMovementInput(Movement);

		//**Move in a new direction at the end of the time line
		//**Value may never hit 1.0 exactly, so when it's close
		if (Value >= 0.999f)
		{
			//**Set the movement vector to a new random direction
			Movement = FVector(FMath::RandRange(-1.0f, 1.0f), FMath::RandRange(-1.0f, 1.0f), 0);			
		}
	}
}

//**On hit fuction
void AEnemyCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor)
	{		//**Only if another character hits
		if (OtherActor->IsA(ACharacter::StaticClass()))
		{		
			//**Launch back in oppisite direction as it's moving. i.e. bounce back off character
			LaunchCharacter(GetActorForwardVector() * -3000, false, false);
		}

		//**else if it's a component pawn
		else if (OtherActor->IsA(AComponentPawn::StaticClass()))
		{			
			//**Add to score
			GameMode->AddToScore(100.0f);
			//**reduce enemy count
			GameMode->ChangeEnemyCount(-1);
			
			//**Spawn the explosion particle emitter
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Explosion, GetActorLocation(), GetActorRotation(), true);
			
			Destroy();
		}

		//**Else, it is an obstacle so change direction
		else
		{		
			//**Reverse movement direction
			Movement = Movement * -1.0f; 
		
		}	
	}	
}

void AEnemyCharacter::BeginDestroy()
{
	Super::BeginDestroy();
		
}