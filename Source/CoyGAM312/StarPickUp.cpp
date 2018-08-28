
// Fill out your copyright notice in the Description page of Project Settings.

#include "StarPickUp.h"
#include "MainCharacter.h"


// Sets default values
AStarPickUp::AStarPickUp()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//**Create a Box component
	UBoxComponent *BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	//**Set the Box's size
	BoxComp->InitBoxExtent(FVector(20.0f, 20.0f, 20.0f));

	//**Set the collision profile name
	BoxComp->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	//**Set the pawn's root component to the Sphere
	RootComponent = BoxComp;

	//**Create the static mesh component
	UStaticMeshComponent* BoxMeshOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Star1"));
	UStaticMeshComponent* BoxMeshTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Star2"));
	UStaticMeshComponent* BoxMeshThree = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Star3"));

	//**Attached the mesh to the sphere component
	BoxMeshOne->SetupAttachment(RootComponent);
	BoxMeshTwo->SetupAttachment(RootComponent);
	BoxMeshThree->SetupAttachment(RootComponent);

	//**Find the 3d model asset for the sphere in the starter content folders
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	//**If the visual asset was successfully found, then set its parameters
	if (CubeVisualAsset.Succeeded())
	{
		//**Set the BoxeMesh to the visual asset we just found
		BoxMeshOne->SetStaticMesh(CubeVisualAsset.Object);
		BoxMeshTwo->SetStaticMesh(CubeVisualAsset.Object);
		BoxMeshThree->SetStaticMesh(CubeVisualAsset.Object);

		//**Set the relative location of the box
		BoxMeshOne->SetRelativeLocation(FVector(0.0f, 0.0f, -1.0f));
		BoxMeshTwo->SetRelativeLocation(FVector(0.0f, 0.0f, -1.0f));
		BoxMeshThree->SetRelativeLocation(FVector(0.0f, 0.0f, -10.0f));

		//**Set the scale size of the box
		BoxMeshOne->SetWorldScale3D(FVector(0.2f, 0.02f, 0.02f));
		BoxMeshTwo->SetWorldScale3D(FVector(0.02f, 0.2f, 0.02f));
		BoxMeshThree->SetWorldScale3D(FVector(0.02f, 0.02f, 0.2f));
	}

	//**Set the material for the mesh
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("/Game/Materials/M_Yellow"));
	if (CubeMaterial.Succeeded())
	{
		BoxMeshOne->SetMaterial(0, CubeMaterial.Object);
		BoxMeshTwo->SetMaterial(0, CubeMaterial.Object);
		BoxMeshThree->SetMaterial(0, CubeMaterial.Object);
	}

	//**Load the sparks particle emmitted asset
	static ConstructorHelpers::FObjectFinder<UParticleSystem> SparksPS(TEXT("/Game/StarterContent/Particles/P_StarSparks.P_StarSparks"));

	//**If successfully found and loaded
	if (SparksPS.Succeeded())
	{
		//**Set the sparks object to the the asset
		Sparks = SparksPS.Object;
	}
	
}

// Called when the game starts or when spawned
void AStarPickUp::BeginPlay()
{
	Super::BeginPlay();

	//**Register the OnHit event
	TScriptDelegate<FWeakObjectPtr> onHit;
	onHit.BindUFunction(this, "OnHit");
	OnActorHit.Add(onHit);

	//**Cast the GameMode
	GameMode = Cast<ACoyGAM312GameModeBase>(GetWorld()->GetAuthGameMode());

	//**Add self to the star count
	GameMode->ChangeStarCount(1);

}

// Called every frame
void AStarPickUp::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//**Rotate the object a bit every tick
	AddActorLocalRotation(FRotator(0.7f, 0.4f, 1.0f));

}

//**On hit event
void AStarPickUp::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//**If other actor exists
	if (OtherActor)
	{
		//**if other actor is main character
		if (OtherActor->IsA(AMainCharacter::StaticClass()))
		{
			//**add to the score
			GameMode->AddToScore(300); 

			//**Reduce star count
			GameMode->ChangeStarCount(-1);

			//**Spawn the explosion particle emitter
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Sparks, GetActorLocation(), GetActorRotation(), true);

			//**Destroy self
			Destroy();
		}
	}
}


