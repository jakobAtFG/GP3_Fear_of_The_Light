// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolManager.h"
#include "GameFramework/SaveGame.h"
#include "GP3SaveGame.generated.h"

USTRUCT()
struct FTerminalData
{
	GENERATED_BODY()

	
	
};

/**
 * 
 */
UCLASS()
class GP3_API UGP3SaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	// PLAYER DATA
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator PlayerRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int PlayerStrikes;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BatteryPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeSinceLitUp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int ClearanceLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<ETool> UnlockedTools;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETool CurrentTool;

	// WORLD DATA
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> UnlockedDoors;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> TerminalOverrides;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, FVector> EnemyStartLocations;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FString> TriggerVolumes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, int> TimerTimings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int NumberOfTerminalsOverridden;
	
	// META DATA

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, bool> SaveTriggerStates;
	
};
