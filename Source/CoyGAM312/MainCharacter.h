// Daniel Coy
// GAM 312 

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "CameraControl.h"
#include "ComponentPawn.h"
#include "MyAIController.h"
#include "CoyGAM312GameModeBase.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/TimelineComponent.h"
#include "EnemyCharacter.h"
#include "HealthPack.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "MainCharacter.generated.h"

UCLASS()
class COYGAM312_API AMainCharacter : public ACharacter
{
	GENERATED_BODY()

	//**Pointer for a Spring Arm component to be used to attach the camera to the player
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm; 

	//**Pointer for the camera component to be attached to the player
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* FixedCamera; 


public:
	// Sets default values for this character's properties
	AMainCharacter(const FObjectInitializer& ObjectInitializer);

private:

	//**Variables for the walk and sprint speeds
	UPROPERTY()
	float WalkSpeed;
	UPROPERTY()
	float SprintSpeed;
	UPROPERTY()
	float JumpVelocity; 

	//**Camera Zoom Variables
	UPROPERTY()
	float FirstPersonCamZoom;
	UPROPERTY()
	float ThirdPersonCamZoom;
	UPROPERTY()
	bool FirstPersonViewToggle; 

	//**Toggle to allow possesion of the other pawn
	UPROPERTY()
	bool AllowPosses; 

	//**Pointer for the actor that is possessed on contact
	UPROPERTY()
	class AActor *PossessedActor;

	//**Stamina Use Timer
	UPROPERTY()
	FTimerHandle StaminaTimer;

	//**POinter to the Game mode for setting UI message
	UPROPERTY()
	ACoyGAM312GameModeBase *GameMode;
	
		
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//**Function for toggling the 1st/3rd person view on the player cam
	UFUNCTION()
	void SetCamView(); 
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//**Player Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxPlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float PlayerHealthPercentage;

	//**Player Stamina
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float PlayerStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float MaxPlayerStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stamina")
	float PlayerStaminaPercentage;

	//**Trigger for showing red flash on screen when damage taken
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	bool RedFlash; 

	//**Timer for Unpossessing
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	FTimerHandle UnpossessTimer;

	//**Property for tracking sphere possession
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Timer")
	bool SpherePossessed; 


	//**Get health for display on UI
	UFUNCTION(BlueprintPure, Category = "Health")
	float GetHealth(); 

	//**Get health for display on UI
	UFUNCTION(BlueprintPure, Category = "Health")
	FText GetHealthIntText();

	UFUNCTION(BlueprintCallable, Category = "Health")
	void UpdateHealth(float HealthChange);

	//**Get Stamina for display on UI
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetStamina();

	//**Get Stamina for display on UI
	UFUNCTION(BlueprintPure, Category = "Stamina")
	FText GetStaminaIntText();

	//**Update stamina
	UFUNCTION()
	void UpdateStamina(float StaminaChange);

	//**Play the red damage flash
	UFUNCTION(BlueprintPure, Category = "Health")
	bool PlayFlash();
	
	//**Function for character to take damage
	UFUNCTION()
	void TakeDamage(float Damage);

	//**Function for player to use stamina
	UFUNCTION()
	void StaminaUseFunction();

	//**Functions for moving the character. 
	//Deviated from the assignment tutorial on function names in order to align with naming in the UE Project Settings axis mapping
	UFUNCTION()
	void MoveForward(float Value);
	UFUNCTION()
	void MoveRight(float Value);
	UFUNCTION()
	void TurnRight(float AxisValue);
	UFUNCTION()
	void TurnUp(float AxisValue);
	UFUNCTION()
	void StartJump();
	UFUNCTION()
	void StopJump();

	//**Functions for executing Sprint action. 
	UFUNCTION()
	void StartSprint();	//**StartSprint is called when the left-Shift is pressed
	UFUNCTION()
	void StopSprint();	//**StopSprint is called when the left-Shift is released
	
	//**Fucntions for collide and overlap
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit); 
	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);

	//**Unpossess Timer Function
	UFUNCTION()
	void UnpossessFunction();
		
	
};