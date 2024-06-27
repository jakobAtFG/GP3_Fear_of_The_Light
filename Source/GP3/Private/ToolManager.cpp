// Fill out your copyright notice in the Description page of Project Settings.

#include "ToolManager.h"
#include "EnhancedInputSubsystems.h"
#include "GP3SaveGame.h"
#include "InputActionValue.h"
#include "GP3/BatteryComponent.h"
#include "GP3/GameManager.h"

// Sets default values for this component's properties
UToolManager::UToolManager()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UToolManager::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//PlayerInputComponent->BindAxis("ItemScrollerUp", this, &UToolManager::IncreaseWeaponIndex);
	//PlayerInputComponent->BindAction(ItemScrollerUp, ETriggerEvent::Triggered, this, &UToolManager::IncreaseWeaponIndex);

	//PlayerInputComponent->BindAxis("ItemScrollerDown", this, &UToolManager::DecreaseWeaponIndex);
	//PlayerInputComponent->BindAction(ItemScrollerDown, ETriggerEvent::Triggered, this, &UToolManager::DecreaseWeaponIndex);
}


// Called when the game starts
void UToolManager::BeginPlay()
{
	Super::BeginPlay();

	BatteryComponent = GetOwner()->GetComponentByClass<UBatteryComponent>();
	CurrentTool = ETool::NONE;

	UGameManager* GameManager = GetWorld()->GetGameInstance<UGameManager>();
	GameManager->OnSaveRequested.AddDynamic(this, &UToolManager::HandleSaveRequested);
	GameManager->OnSaveDataLoaded.AddDynamic(this, &UToolManager::HandleSaveDataLoaded);
}


// Called every frame
void UToolManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (BatteryComponent && CurrentTool != ETool::EMP && !BatteryComponent->Consume(CurrentTool))
	{
		ToolToggleOn = false;
		OnToolToggled.Broadcast(ToolToggleOn, CurrentTool);
		
		switch (CurrentTool)
		{
		case ETool::RADAR:
			SwitchToolRadar(FInputActionValue());
			break;
		case ETool::HEAT_VISION:
			SwitchToolHeatVision(FInputActionValue());
			break;
		}
	}

	// ...
}

void UToolManager::AddTool(ETool Tool)
{
	AvailableTools.Add(Tool);
}

void UToolManager::EquipTool()
{
	ToolToggleOn = false;
	//int currentToolIndex = AvailableTools.IndexOfByKey(CurrentTool);
	//CurrentTool = AvailableTools[currentToolIndex +	1];

	CurrentTool = AvailableTools[Index];


	switch (CurrentTool)
	{
	case ETool::NONE:
		UE_LOG(LogTemp, Warning, TEXT("Equiped tool NONE"));
		break;

	case ETool::EMP:
		UE_LOG(LogTemp, Warning, TEXT("Equiped tool EMP"));
		OnSwitchedToEmp.Broadcast();
		break;

	case ETool::RADAR:
		UE_LOG(LogTemp, Warning, TEXT("Equiped tool RADAR"));
		OnSwitchedToRadar.Broadcast();
		break;

	case ETool::HEAT_VISION:
		UE_LOG(LogTemp, Warning, TEXT("Equiped tool HEAT VISION"));
		OnSwitchedToHeatVision.Broadcast();
	}

	OnToolSwitched.Broadcast(CurrentTool);
}

void UToolManager::SwitchTool(const FInputActionValue& Value)
{
	float Val = Value.Get<float>();

	if (AvailableTools.Num() == 0) return;

	if (CurrentTool == ETool::NONE)
	{
		Index = 0;
	}

	Index += Val;

	if (Index < 0)
	{
		Index = AvailableTools.Num() - 1;
	}
	else if (Index >= AvailableTools.Num())
	{
		Index = 0;
	}

	CurrentTool = AvailableTools[Index];

	EquipTool();
}

void UToolManager::SwitchToolRadar(const FInputActionValue& Value)
{
	if (AvailableTools.Contains(ETool::RADAR))
	{
		if (CurrentTool != ETool::RADAR)
		{
			CurrentTool = ETool::RADAR;
		}
		else if (CurrentTool == ETool::RADAR)
		{
			CurrentTool = ETool::NONE;
		}
		OnSwitchedToRadar.Broadcast();
		OnToolSwitched.Broadcast(CurrentTool);
	}
}

void UToolManager::SwitchToolHeatVision(const FInputActionValue& Value)
{
	if (AvailableTools.Contains(ETool::HEAT_VISION))
	{
		if (CurrentTool != ETool::HEAT_VISION)
		{
			CurrentTool = ETool::HEAT_VISION;
		}
		else if (CurrentTool == ETool::HEAT_VISION)
		{
			CurrentTool = ETool::NONE;
		}
		OnSwitchedToHeatVision.Broadcast();
		OnToolSwitched.Broadcast(CurrentTool);
	}
}

void UToolManager::SwitchToolEmp(const FInputActionValue& Value)
{
	if (AvailableTools.Contains(ETool::EMP))
	{
		if (CurrentTool != ETool::EMP)
		{
			CurrentTool = ETool::EMP;
		}
		else if (CurrentTool == ETool::EMP)
		{
			CurrentTool = ETool::NONE;
		}
		OnSwitchedToEmp.Broadcast();
		OnToolSwitched.Broadcast(CurrentTool);
	}
}

bool UToolManager::bIsHoldingTool()
{
	if (CurrentTool == ETool::RADAR || CurrentTool == ETool::EMP) return true;

	return false;
}

void UToolManager::UnequipTool()
{
	CurrentTool = ETool::NONE;
	OnToolSwitched.Broadcast(CurrentTool);
}

void UToolManager::HandleSaveRequested(UGP3SaveGame* SaveGame)
{
	SaveGame->UnlockedTools = AvailableTools;
	SaveGame->CurrentTool = CurrentTool;
}

void UToolManager::HandleSaveDataLoaded(UGP3SaveGame* SaveGame)
{
	AvailableTools = SaveGame->UnlockedTools;

	switch (SaveGame->CurrentTool)
	{
		case ETool::NONE:
			CurrentTool = ETool::NONE;
			break;
		case ETool::RADAR:
			SwitchToolRadar(FInputActionValue());
			break;
		case ETool::HEAT_VISION:
			SwitchToolHeatVision(FInputActionValue());
			break;
		case ETool::EMP:
			SwitchToolEmp(FInputActionValue());
			break;
	}
};
