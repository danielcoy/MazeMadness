// Daniel Coy
// GAM 312

#include "MainCharacter.h"
#include "CameraControl.h"
#include "Kismet/KismetMathLibrary.h"
#include "TimerManager.h"


// Sets default values
AMainCharacter::AMainCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), WalkSpeed(500.0), SprintSpeed(1500.00), JumpVelocity(400.0f),				//**Initialize the movement speed variables
	FirstPersonCamZoom(-30.0f), ThirdPersonCamZoom(200.0f), FirstPersonViewToggle(true), AllowPosses(true) //**Initialize the camera variables															
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
		
	//**Create the camera spring arm
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm")); 
	//**Attach the spring arm to the RootComponent of the Main Character object
	SpringArm->SetupAttachment(RootComponent); 
	//**Set the relative location of the spring arm to be at character head height
	SpringArm->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight - 15.0f)); 
	//**Set the arm length to the inital camera zoom value
	SpringArm->TargetArmLength = FirstPersonCamZoom;
	//**Tell the spring arm NOT to use the rotation control of the pawn
	SpringArm->bUsePawnControlRotation = true;
	

	//**Create the fixed cam 
	FixedCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FixedCam")); 
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

	//**Set the player to auto posses this pawn on begining of play
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	//**Initialize player stats
	MaxPlayerHealth = 100.0f; 
	PlayerHealth = MaxPlayerHealth; 
	PlayerHealthPercentage = 1.0f;

	MaxPlayerStamina = 100.0f;
	PlayerStamina = MaxPlayerStamina;
	PlayerStaminaPercentage = 1.0f; 
	
	//**Set red flash trigger to false
	RedFlash = false; 

	//**Set sphere possession to false
	SpherePossessed = false; 
		
	
}

// Called when the game starts or when spawned
void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	//**Set the capsule component to generate overlap events
	GetCapsuleComponent()->bGenerateOverlapEvents = true;
	
	//**Add the BeginOverlap() function to the execution list
	TScriptDelegate<FWeakObjectPtr> onOverlap; 
	onOverlap.BindUFunction(this, "BeginOverlap"); 
	OnActorBeginOverlap.Add(onOverlap);
	
	//**Set jump velocity
	GetCharacterMovement()->JumpZVelocity = JumpVelocity; 
	
	//**On start play set the view to 3rd person
	FirstPersonViewToggle = false; 

	//**Cast the GameMode
	GameMode = Cast<ACoyGAM312GameModeBase>(GetWorld()->GetAuthGameMode());

	//**Set an initial UI message
	GameMode->SetUIMessage("Navigate the maze \nand avoid enemies!");

}

// Called every frame
void AMainCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//**if the FirstPersonToggle is true, meaning the view has been set to 1st person
	//** AND the current spring arm length isn't at the 1st person zoom value, then decrease the arm length
	if ((FirstPersonViewToggle) && (SpringArm->TargetArmLength > FirstPersonCamZoom))
	{
		//** Calculate the distance to move the camera so that it take 1 second
		SpringArm->TargetArmLength -= (DeltaTime *(ThirdPersonCamZoom - FirstPersonCamZoom));
	}

	//**else if the toggle is false, meaning view has been set to 3rd person,
	//** AND the spring arm length is not at the 3rd person view level, then increase arm length
	else if ((!FirstPersonViewToggle) && (SpringArm->TargetArmLength < ThirdPersonCamZoom))
	{
		//** Calculate the distance to move the camera so that it take 1 second
		SpringArm->TargetArmLength += (DeltaTime *(ThirdPersonCamZoom - FirstPersonCamZoom));
	}
	
	//**Check the stamina and stop the sprinting if below 0. This will prevent a player from just holding 
	//**button down for infinite sprint. If stamina is 0 or less, and currently sprinting
	if (PlayerStamina <= 0.0f && GetCharacterMovement()->MaxWalkSpeed == SprintSpeed)
	{
		//**Call the stop sprint function
		StopSprint();
		GameMode->SetUIMessage("Out of Stamina!");
	}

	//**Get the current velocity of the player, if it is 0 player isn't moving. If stamina is also less than 100
	if (GetVelocity() == FVector(0.0f, 0.0f, 0.0f) && PlayerStamina < 100.0f)
	{
		//**Recover stamina over 10 seconds
		UpdateStamina((DeltaTime * 100.0f) / 10.0f); 
			
	}	
}

// Called to bind functionality to input
void AMainCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(PlayerInputComponent);

	//**Call the BindAxis function on the Player Input Component and assign the bindings that were created
	//**These bindings currently set to the WASD keys
	PlayerInputComponent->BindAxis("MoveForward", this, &AMainCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMainCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRight", this, &AMainCharacter::TurnRight);
	PlayerInputComponent->BindAxis("TurnUp", this, &AMainCharacter::TurnUp);

	//**Call the Bind Action functions to assign the bindings for the Sprint event.
	//**Defines the event for when the button is Pressed as the StartSprint function, set to left-shift key
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &AMainCharacter::StartSprint);	
	//**Defines the event for when the button is Released as the StopSprint Function,  set to left-shift key
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &AMainCharacter::StopSprint);	
	
	//**Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AMainCharacter::StartJump); 
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AMainCharacter::StopJump);

	//**Bind the action for the SetCamView event to toggle between 1st and 3rd person views, set to the '=' key
	PlayerInputComponent->BindAction("SetCamView", IE_Pressed, this, &AMainCharacter::SetCamView); 
		
}

//**Fucntion called when the Move Forward axis event occurs
void AMainCharacter::MoveForward(float Value)
{
	//**If the Controller and Value objects have values
	if (Controller && Value)
	{
		//**Get the character's forward movement vector. The vector will use the game's 3-D coordinate system
		// to describe the direction of movement. A vector will typically also describe a magnitude, but in 
		// this case it is likely that the vector has been "normalize" in order to simply contain the 
		// directional information of the character's movement. 
		FVector Forward = GetActorForwardVector();

		//**Add movement input using the forward vector and the Value argument
		AddMovementInput(Forward, Value);
	}
}

//**Fucntion called when the Move Right axis event occurs
void AMainCharacter::MoveRight(float Value)
{
	//**If the Controller and Value objects have values
	if (Controller && Value)
	{
		//**Get the character's right movement vector. See comment above regarding the forward vector. 
		FVector Right = GetActorRightVector();

		//**Add movement input using the right vector and the Value argument
		AddMovementInput(Right, Value);
	}
}

//**Turn Right. Keeping this fucntion instead of just calling the YawInput directly from the BindAxis in case I want to add more actions on mouse movement 
void AMainCharacter::TurnRight(float AxisValue)
{
	//Add controller input  in the Yaw dirrection
	AddControllerYawInput(AxisValue); 
}

//**Turn up
void AMainCharacter::TurnUp(float AxisValue)
{
	//**Add pitcj input
	AddControllerPitchInput(AxisValue); 
}

//**Function to start a sprint using the left-Shift key. 
void AMainCharacter::StartSprint()
{
	//**Only allow if stamina above 0
	if (PlayerStamina > 0.0f)
	{
		//**Set the Max Walk Speed variable of the Character Movement component to the SprintSpeed
		GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;

		//**Start timer to use stamina
		GetWorldTimerManager().SetTimer(StaminaTimer, this, &AMainCharacter::StaminaUseFunction, 0.25f, true, -1.0f);
	}

	//**Else out of stamina
	else
	{
		GameMode->SetUIMessage("Out of stamina");
	}
}

//**Function to stop sprint when releasing the left-Shift key
void AMainCharacter::StopSprint()
{
	//**Set the Max Walk Speed variable of the Character Movement component back to the WalkSpeed
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

	//**Clear the stamina use timer
	GetWorldTimerManager().ClearTimer(StaminaTimer);
}

//**Function for jumping
void AMainCharacter::StartJump()
{
	bPressedJump = true;
}

void AMainCharacter::StopJump()
{
	bPressedJump = false;
}


//**Function to toggle between 1st and 3rd person views by changing the length of the spring arm
void AMainCharacter::SetCamView()
{
	//**Toggle the variable
	FirstPersonViewToggle = !FirstPersonViewToggle;	
}

//**Function for hit ditection on blocking objects
void AMainCharacter::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit)
{
	//**If there is an other actor, then check if it's the component pawn. 
	//**This prevents trying to call the IsA() function on a NULL actor, like witha BSP object, which causes a crash
	if (OtherActor)
	{		
		//**If the other actor is a ComponentPawn object AND the AllowPosses is true
		if (OtherActor->IsA(AComponentPawn::StaticClass()) && AllowPosses)
		{
			//**Set allow posses to false, so this action only occurs once
			AllowPosses = false;
			
			//**Display a UI message
			GameMode->SetUIMessage("Sphere Possessed! \nYou can now destroy the enemies!");

			//**Add to score
			GameMode->AddToScore(500.0f);

			//**Get the controller
			AController* controller = GetController();
			//**Unposess the current pawn (AKA the player character)
			controller->UnPossess();

			//**Set the possessed actor
			PossessedActor = OtherActor; 

			//**Cast the PossessedActor to a Pawn object
			APawn *pawn = Cast<APawn>(PossessedActor);
			
			//**Possess the pawn
			controller->Possess(pawn);

			//**Set sphere possession to true
			SpherePossessed = true; 

			//**Set the other actor's root component's rotation to match the character's
			OtherActor->GetRootComponent()->SetWorldRotation(GetActorRotation());

			//**Stop all movement
			GetMovementComponent()->StopMovementImmediately();	
			//**Also stop sprint, in case the player was holding the sprint button at time of possession
			StopSprint();

			//**Set the timer to trigger unpossessing of the other pawn
			GetWorldTimerManager().SetTimer(UnpossessTimer, this, &AMainCharacter::UnpossessFunction, 10.0f, false, -1.0f);	
						
		}	

		//**Else, if other oactor is an Enemy character		
		else if (OtherActor->IsA(AEnemyCharacter::StaticClass()))
		{
			//**Take damage
			TakeDamage(5.0f); 		

			//**Bounce the player back off the enemy character. This gets the enemies forward vector, then
			//**uses that to lanuch the character in that same direction. Effectively bouncing the player
			//**back which ever way the enemy is moving. 
			LaunchCharacter(OtherActor->GetActorForwardVector() * 2000.0f, false, false);
		}
		
	}
	
}

//**Function for the overlap of objects that allow overlap
void AMainCharacter::BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	//**If the OtherActor exists
	if (OtherActor)
	{
		//**If other actor is a health pack and player has less than 100 health
		if (OtherActor->IsA(AHealthPack::StaticClass()) && PlayerHealth < 100.0f)
		{
			//**Cast to health pack object
			AHealthPack *HP = Cast<AHealthPack>(OtherActor);

			//**Add the health value from the pack to player health
			UpdateHealth(HP->GetHealthValue());

			//**Convert health valure to integer for display on screen
			int32 HealthValInt = HP->GetHealthValue(); 

			//**Create message and display
			FString msg = "Received " + FString::FromInt(HealthValInt) + " health!";
			GameMode->SetUIMessage(msg);

			//**Add to score
			GameMode->AddToScore(200.0f);

			//**Destroy the health pack object
			OtherActor->Destroy();

		}

		//**else if health is full
		else if (OtherActor->IsA(AHealthPack::StaticClass()) && PlayerHealth >= 100.0f)
		{
			//**Show message
			GameMode->SetUIMessage("Health already full!");
		}
	}	
}

//**Unpossess timer function
void AMainCharacter::UnpossessFunction()
{
	GameMode->SetUIMessage("UNPOSSESS!");
	//**Get the controller
	
	//**Possessed actor's pawn
	APawn *PossessedPawn = Cast<APawn>(PossessedActor);
	
	//**Possessed pawn controller
	AController* controller = PossessedPawn->GetController();
	
	//**Unposess the current pawn 
	controller->UnPossess();

	//**Caset this to a Pawn object
	APawn *CharPawn = Cast<APawn>(this); 

	//**possess the pawn
	controller->Possess(CharPawn);
	
	//**Destroy the other actor 
	PossessedActor->Destroy();

	//**Now allow to possess again
	AllowPosses = true; 

	//**Clear the timer
	GetWorldTimerManager().ClearTimer(UnpossessTimer);

	//**Set sphere possession to false
	SpherePossessed = false;
}

//**Function for using stamina
void AMainCharacter::StaminaUseFunction()
{
	//**Decrease Stamina
	UpdateStamina(-10.0f);
}

//**Return the health percentage
float AMainCharacter::GetHealth()
{
	return PlayerHealthPercentage; 
}

//**return stamina percentage
float AMainCharacter::GetStamina()
{
	return PlayerStaminaPercentage; 
}

//**Return the health percentage in text format
FText AMainCharacter::GetHealthIntText()
{
	//**Translate health percentage in to 1 - 100 int format
	int32 HP = FMath::RoundHalfFromZero(PlayerHealthPercentage * 100);
	//**Get FString of HP
	FString HP_string = FString::FromInt(HP);
	//**Create he HUD message to display
	FString HealthHUD = HP_string + FString("%"); 
	//**Convert to FText
	FText HPtext = FText::FromString(HealthHUD);
	return HPtext;
}

//**Retrun stamina percentage in text format
FText AMainCharacter::GetStaminaIntText()
{
	//**Translate stamina percentage in to 1 - 100 int format
	int32 Stamina = FMath::RoundHalfFromZero(PlayerStaminaPercentage * 100);\
	//**Get FString of Stamina
	FString Stam_string = FString::FromInt(Stamina);
	//**Create HUD Message to display
	FString StamHUD = Stam_string + FString("%");
	//**Convert to FText and return
	FText Stamtext = FText::FromString(StamHUD);
	return Stamtext;
}

//**Tell UI to play the red flash
bool AMainCharacter::PlayFlash()
{
	//**If RedFlash is true
	if (RedFlash)
	{
		//**turn it false so this doesn't keep repeating itself
		RedFlash = false;

		//**tell UI to play flash
		return true; 
	}

	//**Else tell UI not to play flash
	else
	{
		return false;
	}
}

//**Take Damage
void AMainCharacter::TakeDamage(float Damage)
{
	//**Set the red flash trigger to true
	RedFlash = true; 

	//**Update the health value by negative Damage
	UpdateHealth(-Damage);

}

void AMainCharacter::UpdateHealth(float HealthChange)
{
	//**Add the health change to player health
	PlayerHealth += HealthChange;
	//**Clamp health to between 0 and the max health
	PlayerHealth = FMath::Clamp(PlayerHealth, 0.0f, MaxPlayerHealth);
	//**Update the health percentage
	PlayerHealthPercentage = PlayerHealth / MaxPlayerHealth;
}

void AMainCharacter::UpdateStamina(float StaminaChange)
{
	//**Add stamina change value 
	PlayerStamina += StaminaChange;
	//**Clamp stamina to between 0 and Max stamina
	PlayerStamina = FMath::Clamp(PlayerStamina, 0.0f, MaxPlayerStamina);
	//**Update stamina percentage
	PlayerStaminaPercentage = PlayerStamina / MaxPlayerStamina;
}

