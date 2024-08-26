// Fill out your copyright notice in the Description page of Project Settings.


#include "GameManager.h"

#include "GP3SaveGame.h"


void UGameManager::Init()
{
	Super::Init();

	SaveGame = Cast<UGP3SaveGame>(UGameplayStatics::CreateSaveGameObject(UGP3SaveGame::StaticClass()));

	OnSaveCompleteDelegate.BindLambda([this](const FString& SlotName, const int32 UserIndex, bool bSuccess)
	{
		bIsSaving = false;
		if (bSuccess)
		{
			UE_LOG(LogTemp, Warning, TEXT("Save complete"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Save failed"));
		}
	});

}

void UGameManager::RequestSave(bool bAsync)
{
	if (bIsLoading || bIsSaving)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot save while loading or saving"));
		return;
	}

	bIsSaving = true;
	
	OnSaveRequested.Broadcast(SaveGame);

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);

	if (bAsync)
	{
		UGameplayStatics::AsyncSaveGameToSlot(SaveGame, LevelName, 0, OnSaveCompleteDelegate);
	}
	else
	{
		UGameplayStatics::SaveGameToSlot(SaveGame, LevelName, 0);
		bIsSaving = false;
	}
}

void UGameManager::RequestLoad()
{
	if (bIsLoading || bIsSaving)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot load while loading or saving"));
		return;
	}
	
	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	
	if (!UGameplayStatics::DoesSaveGameExist(LevelName, 0))
	{
		UE_LOG(LogTemp, Warning, TEXT("No save game found"));
		return;
	}

	bIsLoading = true;
	
	bIsGameEnded = false;
	
	SaveGame = Cast<UGP3SaveGame>(UGameplayStatics::LoadGameFromSlot(LevelName, 0));
	
	OnSaveDataLoaded.Broadcast(SaveGame);

	bIsLoading = false;
}

bool UGameManager::SetSteamIntStat(FName StatName, int32 Value, bool bOverwrite = false)
{
    /*This method sets the value of a Steam Stat of the current Steam user. It has three input variables: 
    StatName - the name of the stat
    Value - the value of the stat
    bOverwrite - a bool to determine if the Steam stat value should be overwritten or incremented by the inputted value
    
    Intended behavior: The method should be blueprintcallable. 
    If bOverwrite is true, the new value of the stat should be the inputted Value, overriding the old value.
    If bOverwrite is false, the new value of the stat should be the old value of the stat plus the inputted Value. That is, we need to get the current value from Steam and then add the inputted value to it before setting it again. 
        If the old value cannot be read or is 0, the new value should simply be set to the inputted Value.*/

    /*IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get(STEAM_SUBSYSTEM);
    if (OnlineSubsystem == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("Steam Online Subsystem not found."));
        return;
    }

    IOnlineStatsPtr StatsInterface = OnlineSubsystem->GetStatsInterface();
    if (!StatsInterface.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Stats interface not available."));
        return;
    }

    IOnlineIdentityPtr IdentityInterface = OnlineSubsystem->GetIdentityInterface();
    TSharedPtr<const FUniqueNetId> UserId = IdentityInterface->GetUniquePlayerId(0);
    if (!UserId.IsValid())
    {
        UE_LOG(LogTemp, Warning, TEXT("Failed to get User ID."));
        return;
    }

    // If Overwrite is true, update the stat directly
    if (bOverwrite)
    {
        FOnlineStatsUserUpdatedStats UserUpdatedStats(UserId.ToSharedRef());
        FOnlineStatUpdate StatUpdate;
        StatUpdate.Set(Value, FOnlineStatUpdate::EOnlineStatModificationType::Set); // ChatGPT suggested StatUpdate.SetByInteger(Value); for this but that function doesn't exist
        UserUpdatedStats.Stats.Add(StatName, StatUpdate);
        StatsInterface->UpdateStats(UserUpdatedStats);
    }
    else
    {
        // If Overwrite is false, query the current stats first
        FOnlineStatsQueryOptions QueryOptions;
        QueryOptions.AddUser(UserId.ToSharedRef());

        StatsInterface->QueryStats(QueryOptions, FOnOnlineStatsQueryComplete::CreateLambda(
            [this, Value, StatName, StatsInterface, UserId](const FOnlineError& Result, const TArray<FOnlineStatsUserStats>& QueriedStats)
            {
                if (Result.WasSuccessful() && QueriedStats.Num() > 0)
                {
                    int32 OldValue = 0;
                    const FOnlineStatValue* StatValue = QueriedStats[0].Stats.Find(StatName);
                    if (StatValue && StatValue->GetValue(OldValue))
                    {
                        int32 NewValue = OldValue + Value;

                        FOnlineStatsUserUpdatedStats UserUpdatedStats(UserId.ToSharedRef());
                        FOnlineStatUpdate StatUpdate;
                        StatUpdate.SetByInteger(NewValue);
                        UserUpdatedStats.Stats.Add(StatName, StatUpdate);
                        StatsInterface->UpdateStats(UserUpdatedStats);
                    }
                    else
                    {
                        UE_LOG(LogTemp, Warning, TEXT("Failed to retrieve current stat value. Cannot update."));
                    }
                }
                else
                {
                    UE_LOG(LogTemp, Warning, TEXT("Failed to query stats."));
                }
            }
        ));
    }*/
}

UGameManager::UGameManager()
{
}
