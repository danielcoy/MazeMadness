//**This class is for the sphere objects that the players possess to destroy enemies

#include "ComponentPawn.h"

// Sets default values
AComponentPawn::AComponentPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//**Create a sphere component
	USphereComponent* Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
	//**Set the sphere's radius
	Sphere->InitSphereRadius(50.0f);
		
	//**Set the collision profile name
	Sphere->BodyInstance.SetCollisionProfileName(TEXT("Pawn"));
	
	//**Set the pawn's root component to the Sphere
	RootComponent = Sphere;
	
	//**Create the static mesh component
	UStaticMeshComponent* SphereMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Visual")); 
	//**Attached the mesh to the sphere component
	SphereMesh->SetupAttachment(RootComponent); 
	
	//**Find the 3d model asset for the sphere in the starter content folders
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Sphere.Shape_Sphere")); 
	//**If the visual asset was successfully found, then set its parameters
	if (SphereVisualAsset.Succeeded())
	{
		//**Set the SphereMesh to the visual asset we just found
		SphereMesh->SetStaticMesh(SphereVisualAsset.Object);
		//**Set the relative location of the sphere
		SphereMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -40.0f)); 
		//**Set the scale size of the sphere
		SphereMesh->SetWorldScale3D(FVector(0.8f)); 
	}

	//**Set the material for the mesh
	static ConstructorHelpers::FObjectFinder<UMaterialInterface> SphereMaterial(TEXT("/Game/Materials/M_SphereMat"));
	if (SphereMaterial.Succeeded())
	{
		SphereMesh->SetMaterial(0, SphereMaterial.Object);
	}
		
	
	//**Create a camera component for the pawn
	//**Create the camera spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("PawnCameraSpringArm"));
	//**Attach the spring arm to the RootComponent of the Main Character object
	SpringArm->SetupAttachment(RootComponent);
	//**Set the relative location of the spring arm to be at center of sphere
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, 40.0f));
	//**Set the arm length to the inital camera zoom value
	SpringArm->TargetArmLength = 300.0f;
	//**Tell the spring arm to use the rotation control of the pawn
	SpringArm->bUsePawnControlRotation = false;


	//**Create the fixed cam 
	FixedCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PawnFixedCam"));
	//**Attached the fixed camera to the Spring Arm
	FixedCamera->SetupAttachment(SpringArm);
	//**Set the camera's location. Not changing it for now
	FixedCamera->RelativeLocation = FVector(0.0f, 0.0f, 0.0f);
	//**Let the pawn control the rotation
	FixedCamera->bUsePawnControlRotation = false;
	//**Set projection mode
	FixedCamera->SetProjectionMode(ECameraProjectionMode::Perspective);
	//**Set the field of view for the camera
	FixedCamera->SetFieldOfView(90.0f);
	//**Set Aspect ratio
	FixedCamera->SetAspectRatio(1.777778f);
	//**Constrain the aspect ratio
	FixedCamera->bConstrainAspectRatio = true;

	//**Create an instance of the MyPawnMovementComponent and update the root
	MovementComponent = CreateDefaultSubobject<UMyPawnMovementComponent>(TEXT("MyPawnMovementComponent"));
	//MovementComponent->UpdatedComponent = RootComponent;
		
			
}

// Called when the game starts or when spawned
void AComponentPawn::BeginPlay()
{
	Super::BeginPlay();

	//Register the OnHit function for collisions
	TScriptDelegate<FWeakObjectPtr> onHit;
	onHit.BindUFunction(this, "OnHit");
	OnActorHit.Add(onHit);
	
	//**If the movement componenet exists, then update the root
	if (MovementComponent)
	{
		MovementComponent->UpdatedComponent = RootComponent;
	}		

	//**Cast the GameMode
	GameMode = Cast<ACoyGAM312GameModeBase>(GetWorld()->GetAuthGameMode());

	
		
}

// Called every frame
void AComponentPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);	
}

// Called to bind functionality to input
void AComponentPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//**Bind the movement and turn axes
	PlayerInputComponent->BindAxis("MoveForward", this, &AComponentPawn::MoveForward); 
	PlayerInputComponent->BindAxis("MoveRight", this, &AComponentPawn::MoveRight); 
	PlayerInputComponent->BindAxis("TurnRight", this, &AComponentPawn::TurnRight);
	PlayerInputComponent->BindAxis("TurnUp", this, &AComponentPawn::TurnUp); 
}

//**Override for the get movement component function
UPawnMovementComponent* AComponentPawn::GetMovementComponent() const
{
	return MovementComponent; 
}

//**Move Forward
void AComponentPawn::MoveForward(float AxisValue)
{
	//**if the movement component exists, and it has been updated to the root component
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		//**Add the input vector value to the forward movement
		MovementComponent->AddInputVector(GetActorForwardVector() * AxisValue); 
	}
}

//**More right
void AComponentPawn::MoveRight(float AxisValue)
{
	//**if the movement component exists, and it has been updated to the root component
	if (MovementComponent && (MovementComponent->UpdatedComponent == RootComponent))
	{
		//**Add the input vector value to the right movement
		MovementComponent->AddInputVector(GetActorRightVector() * AxisValue);
	}	
}

//**Turn Right
void AComponentPawn::TurnRight(float AxisValue)
{
	
	//**Get the current rotation of the actor
	FRotator NewPawnRotation = GetActorRotation();
	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation(); 
	//**Add the value of the axis to the Yaw of the rotation
	NewPawnRotation.Yaw += AxisValue; 
	NewSpringArmRotation.Yaw += AxisValue;
	//**Set the actor rotation to the new rotation value
	SetActorRotation(NewPawnRotation); 
	SpringArm->SetWorldRotation(NewSpringArmRotation);
	
	//AddControllerYawInput(AxisValue); 
}

//**Turn up
void AComponentPawn::TurnUp(float AxisValue)
{
	//**Invert the axis value
	AxisValue *= -1;

	//**Get the current rotation the spring arm
	FRotator NewSpringArmRotation = SpringArm->GetComponentRotation();

	//**If the pitch of the current rotation is between -80 and 80
	if (NewSpringArmRotation.Pitch <= 80 && NewSpringArmRotation.Pitch >= -80)
	{
		//**Add the axis value to the pitch
		NewSpringArmRotation.Pitch += AxisValue;
		//**Set the new roations
		SpringArm->SetWorldRotation(NewSpringArmRotation);
	}

	//Else, the pitch is outside the range
	else
	{
		//**If pitch is greater than 80, reset it to 80
		if (NewSpringArmRotation.Pitch > 80)
		{
			NewSpringArmRotation.Pitch = 80;
		}

		//**else if it is less than -80 reset it to -80
		else if (NewSpringArmRotation.Pitch < -80)
		{
			NewSpringArmRotation.Pitch = -80;
		}
		//**Set the new roations
		SpringArm->SetWorldRotation(NewSpringArmRotation);
	}
	
	//AddControllerPitchInput(AxisValue); 
}

//**Set the movement speed of the pawn
void AComponentPawn::SetSpeed(float Speed)
{
	MovementComponent->MoveSpeed = Speed; 
}

void AComponentPawn::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//**Check if other actor & component exists
	if (OtherActor)
	{
		//**If the other actor is an Enemy Character
		if (OtherActor->IsA(AEnemyCharacter::StaticClass()))
		{
			//**Destroy enemy actor
			//OtherActor->Destroy();
				

		}
				
	}
}
