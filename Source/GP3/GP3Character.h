// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
// #include "BatteryComponent.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "HealthComponent.h"
#include "Components/SphereComponent.h"
#include "GP3Character.generated.h"

class UGP3SaveGame;
class AInteractableActor;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
class UBatteryComponent;
class UToolManager;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config = Game)
class AGP3Character : public ACharacter
{
	GENERATED_BODY()

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;

	/** Jump Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;


	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintingAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchToolAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* TurnOnToolAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchToRadarAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchToHeatVisionAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SwitchToEmpAction;

public:
	AGP3Character();
	void FindTargetInteractableActor();

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay();

public:
	/* Player Strikes/Mistakes */
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = "Stats")
	int Strikes;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;


	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Bool for AnimBP to switch to another animation set */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Weapon)
	bool bHasRifle;

	/** Setter to set the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	void SetHasRifle(bool bNewHasRifle);

	/** Getter for the bool */
	UFUNCTION(BlueprintCallable, Category = Weapon)
	bool GetHasRifle();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UHealthComponent* HealthComponent = nullptr;

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	// interaction detection
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractionDetection = nullptr;

	UPROPERTY(EditAnywhere)
	float DetectionLength = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBatteryComponent* BatteryComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UToolManager* ToolManager = nullptr;

	UPROPERTY(EditAnywhere, Category = Moevement)
	bool Crouched;


	UPROPERTY(EditAnywhere, Category = Moevement)
	bool Sprinting;


	UPROPERTY(EditAnywhere, Category = Moevement)
	bool Walking;


	UPROPERTY(EditAnywhere, Category = Moevement)
	float WalkingMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Moevement)
	float SprintingMoveSpeed;

	UPROPERTY(EditAnywhere, Category = Moevement)
	float CrouchingMoveSpeed;


	UPROPERTY(VisibleAnywhere)
	AInteractableActor* TargetInteractableActor = nullptr;

	UFUNCTION()
	void HandleDetectionStartingOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& Hit
	);

	UFUNCTION()
	void HandleDetectionEndingOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);

	UPROPERTY(VisibleAnywhere)
	int InteractbleInRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsDebugging = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSettings")
	bool PlayerIsLitUp = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSettings")
	float InvertHorizontal = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSettings")
	float InvertVertical = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSettings")
	float HorizontalSensitivity = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerSettings")
	float VerticalSensitivity = 1;

	UPROPERTY(EditDefaultsOnly)
	FVector WherePlayerIsLooking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FHitResult HitResult2;

	UPROPERTY(EditDefaultsOnly);
	TSubclassOf<UUserWidget> PauseMenuWidgetClass;

	UFUNCTION()
	void ToolToggle(const FInputActionValue& Value);

	UFUNCTION()
	void HandleSaveRequested(UGP3SaveGame* SaveGame);

	UFUNCTION()
	void HandleSaveDataLoaded(UGP3SaveGame* SaveGame);

protected:
	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	/** Called for pause input */
	void TogglePause(const FInputActionValue& Value);

	/** Called for crouch input*/
	void CrouchA(const FInputActionValue& Value);

	/** Called for sprint input*/

	void Sprint(const FInputActionValue& Value);


	void Interaction(const FInputActionValue& Value);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UPROPERTY(VisibleAnywhere)
	FVector ViewOrigin;

	UPROPERTY(VisibleAnywhere)
	FVector ViewForward;

	UPROPERTY(VisibleAnywhere)
	FRotator ViewRotation;

	UPROPERTY(VisibleAnywhere)
	UUserWidget* PauseMenuWidget;
};
