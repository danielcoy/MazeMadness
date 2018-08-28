// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthPack.h"


// Sets default values
AHealthPack::AHealthPack() : HealthValue(20.0f)		//**Initialize the health value
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//**Create a Box component
	UBoxComponent *BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("RootComponent"));
	//**Set the Box's size
	BoxComp->InitBoxExtent(FVector(50.0f, 50.0f, 50.0f)); 

	//**Set the collision profile name
	BoxComp->BodyInstance.SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	//**Set the pawn's root component to the Sphere
	RootComponent = BoxComp;

	//**Create the static mesh component
	UStaticMeshComponent* BoxMeshOne = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cross1"));
	UStaticMeshComponent* BoxMeshTwo = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cross2"));
	//**Attached the mesh to the sphere component
	BoxMeshOne->SetupAttachment(RootComponent);
	BoxMeshTwo->SetupAttachment(RootComponent);

	//**Find the 3d model asset for the sphere in the starter content folders
	static ConstructorHelpers::FObjectFinder<UStaticMesh> CubeVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	//**If the visual asset was successfully found, then set its parameters
	if (CubeVisualAsset.Succeeded())
	{
		//**Set the BoxeMesh to the visual asset we just found
		BoxMeshOne->SetStaticMesh(CubeVisualAsset.Object);
		BoxMeshTwo->SetStaticMesh(CubeVisualAsset.Object);
		//**Set the relative location of the boxes
		BoxMeshOne->SetRelativeLocation(FVector(0.0f, 0.0f, -20.0f));
		BoxMeshTwo->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f));
		//**Set the scale size of the sphere
		BoxMeshOne->SetWorldScale3D(FVector(0.3f, 0.7f, 0.3f));
		BoxMeshTwo->SetWorldScale3D(FVector(0.3f, 0.3f, 0.7f));
				
	}

	//**Set the material for the mesh
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> CubeMaterial(TEXT("/Game/Materials/M_Red"));
	if (CubeMaterial.Succeeded())
	{
		BoxMeshOne->SetMaterial(0, CubeMaterial.Object);
		BoxMeshTwo->SetMaterial(0, CubeMaterial.Object);
	}

}

// Called when the game starts or when spawned
void AHealthPack::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AHealthPack::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//**Rotate the object a bit every tick
	AddActorLocalRotation(FRotator(0.0f, 1.0f, 0.0));
	
}

float AHealthPack::GetHealthValue()
{
	return HealthValue;
}

