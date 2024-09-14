// Copyright Epic Games, Inc. All Rights Reserved.

#include "GP3Character.h"
#include "GP3Projectile.h"
#include "GameManager.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GP3.h"
#include "InputActionValue.h"
#include "InteractableActor.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"
#include "BatteryComponent.h"
#include "GP3SaveGame.h"
#include "ToolManager.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/CharacterMovementComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AGP3Character

AGP3Character::AGP3Character()
{
	PrimaryActorTick.bCanEverTick = true;

	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	//Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	HealthComponent = CreateDefaultSubobject<UHealthComponent>("Health Component");
	HealthComponent->OnDeath.AddDynamic(this, &AGP3Character::HandleDeath);

	InteractionDetection = CreateDefaultSubobject<USphereComponent>("Interaction Detection");
	InteractionDetection->SetSphereRadius(DetectionLength);
	InteractionDetection->SetupAttachment(RootComponent);
	InteractionDetection->OnComponentBeginOverlap.AddDynamic(this, &AGP3Character::HandleDetectionStartingOverlap);
	InteractionDetection->OnComponentEndOverlap.AddDynamic(this, &AGP3Character::HandleDetectionEndingOverlap);
	InteractionDetection->SetCollisionObjectType(ECC_InteractionDetection);

	BatteryComponent = CreateDefaultSubobject<UBatteryComponent>("Battery Component");
	BatteryComponent->OwnerCharacter = this;

	ToolManager = CreateDefaultSubobject<UToolManager>("Tool Manager Component");
	ToolManager->OwnerCharacter = this;
}

void AGP3Character::FindTargetInteractableActor()
{
	// nothing in the range
	if (InteractbleInRange <= 0)
	{
		TargetInteractableActor = nullptr;
		return;
	}

	FHitResult HitResult;

	if(!GetController()) return;
	GetController()->GetPlayerViewPoint(ViewOrigin, ViewRotation);
	ViewForward = ViewRotation.Quaternion().GetForwardVector();
	GetWorld()->LineTraceSingleByChannel(
		HitResult,
		ViewOrigin,
		ViewOrigin + ViewForward * DetectionLength,
		ECC_Interaction
	);
	
	//DrawDebugLine(GetWorld(), ViewOrigin, ViewOrigin + ViewForward * DetectionLength, FColor::Red, false, -1, 0, 5);

	// something in the range but not in front player
	if (!HitResult.bBlockingHit)
	{
		if (bIsDebugging)
		{
			DrawDebugLine(
				GetWorld(),
				ViewOrigin,
				ViewOrigin + ViewForward * DetectionLength,
				FColor::Red,
				false,
				-1,
				0,
				5.f);
		}
		if (TargetInteractableActor)
		{
			TargetInteractableActor->CloseInfoOnScreen();
			TargetInteractableActor = nullptr;
		}
	}


	// something in the range And in front player
	else
	{
		if (bIsDebugging)
		{
			DrawDebugLine(
				GetWorld(),
				ViewOrigin,
				HitResult.ImpactPoint,
				FColor::Green,
				false,
				-1,
				0,
				5.f);
		}

		AActor* HitActor = HitResult.Component->GetOwner();

		if (!HitActor) return;
		if (!HitActor->IsA(AInteractableActor::StaticClass())) return;
		// UE_LOG(LogTemp, Warning, TEXT("%s is the target interaction object"), *HitActor->GetName());

		// check if there are something visible in between
		
		FVector Start = GetActorLocation();
		FVector End = HitResult.Location;
    
		FHitResult VisibleHitResult;
		FCollisionQueryParams Params;
		Params.AddIgnoredActor(this);
		bool bHit = GetWorld()->LineTraceSingleByChannel(VisibleHitResult, Start, End, ECC_Visibility, Params);
		// if something is blocking in between clear anything of the TargetInteractableActor
		if (bHit && VisibleHitResult.GetActor() != HitActor)
		{
			if (bIsDebugging)
			{
				DrawDebugLine(
					GetWorld(),
					ViewOrigin,
					HitResult.ImpactPoint,
					FColor::Blue,
					false,
					-1,
					0,
					6.f);
			}
			if (TargetInteractableActor)
			{
				AInteractableActor* OldInteractableActor = TargetInteractableActor;
				OldInteractableActor->CloseInfoOnScreen();
				TargetInteractableActor = nullptr;
			}
			return;
		}
		
		
		if (!TargetInteractableActor || TargetInteractableActor != HitActor)
		{
			if (TargetInteractableActor)
			{
				AInteractableActor* OldInteractableActor = TargetInteractableActor;
				OldInteractableActor->CloseInfoOnScreen();
			}
			TargetInteractableActor = Cast<AInteractableActor>(HitActor);
			TargetInteractableActor->ShowInfoOnScreen();
		}
	}
	// UE_LOG(LogTemp, Warning, TEXT("ViewForward: %s"), *ViewForward.ToString());
}

void AGP3Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	// float time = GetWorld()->GetTimeSeconds();
	// if(time<2.f)
	// {
	// 	InteractionDetection->SetSphereRadius(DetectionLength * time/2.f);
	// }
	InteractionDetection->SetSphereRadius(DetectionLength);

	FindTargetInteractableActor();
}

void AGP3Character::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();


	WherePlayerIsLooking = ViewOrigin + ViewRotation.Quaternion().GetForwardVector() * DetectionLength;

	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<
			UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TArray<AActor*> InitialOverlappingInteractable;
	TSubclassOf<AInteractableActor> Filter;
	InteractionDetection->GetOverlappingActors(InitialOverlappingInteractable, Filter);
	InteractbleInRange = InitialOverlappingInteractable.Num();
	// InteractionDetection->SetSphereRadius(0.f);

	GetCharacterMovement()->MaxWalkSpeedCrouched = CrouchingMoveSpeed;

	Cast<UGameManager>(GetGameInstance())->OnSaveRequested.AddDynamic(this, &AGP3Character::HandleSaveRequested);
	Cast<UGameManager>(GetGameInstance())->OnSaveDataLoaded.AddDynamic(this, &AGP3Character::HandleSaveDataLoaded);
}

void AGP3Character::ToolToggle(const FInputActionValue& Value)
{
	//ToolManager->ToolToggleOn = Value.Get<bool>();
	ToolManager->ToolToggleOn = !ToolManager->ToolToggleOn;
	UE_LOG(LogTemp, Warning, TEXT("ToolToggleOn: %s"), ToolManager->ToolToggleOn ? TEXT("true") : TEXT("false"))
	ToolManager->OnToolToggled.Broadcast(ToolManager->ToolToggleOn, ToolManager->GetCurrentTool());
}

void AGP3Character::HandleSaveRequested(UGP3SaveGame* SaveGame)
{
	SaveGame->PlayerLocation = GetActorLocation();
	SaveGame->PlayerRotation = GetActorRotation();
	SaveGame->PlayerHealth = HealthComponent->CurrentHealth;
	SaveGame->PlayerStrikes = Strikes;
	SaveGame->BatteryPower = BatteryComponent->CurrentPower;
}

void AGP3Character::HandleSaveDataLoaded(UGP3SaveGame* SaveGame)
{
	SetActorLocation(SaveGame->PlayerLocation);
	Controller->SetControlRotation(SaveGame->PlayerRotation);
	HealthComponent->CurrentHealth = SaveGame->PlayerHealth;
	Strikes = SaveGame->PlayerStrikes;
	BatteryComponent->CurrentPower = SaveGame->BatteryPower;
}

//////////////////////////////////////////////////////////////////////////// Input

void AGP3Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGP3Character::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, BatteryComponent,
		                                   &UBatteryComponent::PauseRechargeWhenMoving);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGP3Character::Look);
		// EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, BatteryComponent,
		//                                    &UBatteryComponent::PauseRechargeWhenLookingAround);


		EnhancedInputComponent->BindAction(SprintingAction, ETriggerEvent::Started, this, &AGP3Character::Sprint);
		EnhancedInputComponent->BindAction(SprintingAction, ETriggerEvent::Completed, this, &AGP3Character::Sprint);


		// Pausing
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AGP3Character::TogglePause);

		//Crouching
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Started, this, &AGP3Character::CrouchA);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &AGP3Character::CrouchA);

		EnhancedInputComponent->BindAction(InteractionAction, ETriggerEvent::Triggered, this,
		                                   &AGP3Character::Interaction);

		// Gadgets
		EnhancedInputComponent->BindAction(SwitchToolAction, ETriggerEvent::Triggered, ToolManager,
		                                   &UToolManager::SwitchTool);
		EnhancedInputComponent->BindAction(TurnOnToolAction, ETriggerEvent::Started, this, &AGP3Character::ToolToggle);

		EnhancedInputComponent->BindAction(SwitchToRadarAction, ETriggerEvent::Started, ToolManager,
		                                   &UToolManager::SwitchToolRadar);
		EnhancedInputComponent->BindAction(SwitchToHeatVisionAction, ETriggerEvent::Started, ToolManager,
		                                   &UToolManager::SwitchToolHeatVision);
		EnhancedInputComponent->BindAction(SwitchToEmpAction, ETriggerEvent::Started, ToolManager,
		                                   &UToolManager::SwitchToolEmp);


		/*EnhancedInputComponent->BindAction(ToggleRadarAction, ETriggerEvent::Triggered, this,
			&AGP3Character::ToggleRadar);
		EnhancedInputComponent->BindAction(ToggleNightVisionAction, ETriggerEvent::Triggered, this, 
			&AGP3Character::ToggleNightVision);*/
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

void AGP3Character::HandleDeath()
{
	UGameManager* GameManager = Cast<UGameManager>(GetWorld()->GetGameInstance());
	if (GameManager)
	{
		GameManager->OnLoseGame();
	}
}

void AGP3Character::HandleDetectionStartingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                                   UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                                   bool bFromSweep, const FHitResult& Hit)
{
	if (OtherActor->IsA(AInteractableActor::StaticClass()))
	{
		UE_LOG(LogTemp, Warning, TEXT("%s is in range of %s"), *OtherActor->GetName(), *GetName());
		Cast<AInteractableActor>(OtherActor)->bIsInteractable = true;
		InteractbleInRange++;
	}
}

void AGP3Character::HandleDetectionEndingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AInteractableActor::StaticClass()))
	{
		//UE_LOG(LogTemp, Warning, TEXT("%s is out of range of %s"), *OtherActor->GetName(), *GetName());
		AInteractableActor* OtherInteractableActor = Cast<AInteractableActor>(OtherActor);
		OtherInteractableActor->CloseInfoOnScreen();
		Cast<AInteractableActor>(OtherActor)->bIsInteractable = false;
		InteractbleInRange--;
	}
}

void AGP3Character::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement 
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
		// if(BatteryComponent) BatteryComponent->bIsRecharging = false;
	}
}


void AGP3Character::CrouchA(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (!Crouched)
		{
			// GetCharacterMovement()->MaxWalkSpeed = CrouchingMoveSpeed;
			// Crouch is controlled by MaxWalkSpeedCrouch
			Crouched = true;
			Crouch();
			UE_LOG(LogTemp, Warning, TEXT("Crouching"));
		}
		else
		{
			// GetCharacterMovement()->MaxWalkSpeed = WalkingMoveSpeed;
			// Crouch is controlled by MaxWalkSpeedCrouch
			Crouched = false;
			UnCrouch();
			UE_LOG(LogTemp, Warning, TEXT("UnCrouching"));
		}
	}
}

void AGP3Character::Sprint(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (!Sprinting)
		{
			GetCharacterMovement()->MaxWalkSpeed = SprintingMoveSpeed;
			Sprinting = true;
			UE_LOG(LogTemp, Warning, TEXT("Sprinting"));
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = WalkingMoveSpeed;
			Sprinting = false;
		}
	}
}


void AGP3Character::Interaction(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (TargetInteractableActor)
		{
			TargetInteractableActor->Interaction();
		}
	}
}

void AGP3Character::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X * InvertHorizontal * HorizontalSensitivity);
		AddControllerPitchInput(LookAxisVector.Y * InvertVertical * VerticalSensitivity);
	}
}

void AGP3Character::TogglePause(const FInputActionValue& Value)
{
	bool bGameWillPause = !UGameplayStatics::IsGamePaused(this);

	UGameplayStatics::SetGamePaused(GetWorld(), bGameWillPause);
	APlayerController* PlayerController = Cast<APlayerController>(Controller);

	if (bGameWillPause && PauseMenuWidgetClass)
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
		PlayerController->bShowMouseCursor = true;
		PauseMenuWidget = CreateWidget<UUserWidget>(GetWorld(), PauseMenuWidgetClass);
		PauseMenuWidget->AddToViewport();
	}
	else if (PauseMenuWidget)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->bShowMouseCursor = false;
		PauseMenuWidget->RemoveFromParent();
	}
}

void AGP3Character::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AGP3Character::GetHasRifle()
{
	return bHasRifle;
}
