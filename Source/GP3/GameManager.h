// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
/*#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Interfaces/OnlineStatsInterface.h"
#include "Interfaces/OnlineIdentityInterface.h"
#include "OnlineStats.h"*/
#include "GameManager.generated.h"

/**
 * 
 */

class UGP3SaveGame;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveRequested, UGP3SaveGame*, SaveGame);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSaveDataLoaded, UGP3SaveGame*, SaveGame);

UCLASS()
class GP3_API UGameManager : public UGameInstance
{
	GENERATED_BODY()
public:

	virtual void Init() override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnWinGame();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void OnLoseGame();

	UFUNCTION(BlueprintCallable)
	void RequestSave(bool bAsync = true);

	UFUNCTION(BlueprintCallable)
	void RequestLoad();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsGameEnded = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UGP3SaveGame* SaveGame = nullptr;

	UPROPERTY(BlueprintAssignable);
	FOnSaveRequested OnSaveRequested;

	UPROPERTY(BlueprintAssignable);
	FOnSaveDataLoaded OnSaveDataLoaded;

	/*UFUNCTION(BlueprintCallable, Category = "Steam")
	static bool SetSteamIntStat(FName StatName, int32 Value, bool Overwrite = false);*/
	
private:
	UGameManager();

	bool bIsSaving = false;

	bool bIsLoading = false;

	FAsyncSaveGameToSlotDelegate OnSaveCompleteDelegate;
};
