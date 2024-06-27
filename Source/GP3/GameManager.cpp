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

UGameManager::UGameManager()
{
}
