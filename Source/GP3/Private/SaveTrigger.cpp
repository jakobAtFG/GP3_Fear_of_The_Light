// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveTrigger.h"

#include "Components/BoxComponent.h"
#include "GP3/GameManager.h"
#include "GP3SaveGame.h"
#include "GP3/GP3Character.h"

// Sets default values
ASaveTrigger::ASaveTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	RootComponent = BoxComponent;
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &ASaveTrigger::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ASaveTrigger::BeginPlay()
{
	Super::BeginPlay();

	UGameManager* GameManager = Cast<UGameManager>(GetGameInstance());

	if (bSaveOnlyOnce)
	{
		GameManager->OnSaveRequested.AddDynamic(this, &ASaveTrigger::HandleSaveRequested);
		GameManager->OnSaveDataLoaded.AddDynamic(this, &ASaveTrigger::HandleSaveDataLoaded);
	}
}

// Called every frame
void ASaveTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASaveTrigger::OnOverlapBegin(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
	)
{
	if (!(bSaveOnlyOnce && bHasSaved) && OtherActor->IsA<AGP3Character>())
	{
		bHasSaved = true;
		Cast<UGameManager>(GetGameInstance())->RequestSave();
	}
}

void ASaveTrigger::HandleSaveRequested(UGP3SaveGame* SaveGame)
{
	FString Name = SaveID;
	
	if (SaveGame->SaveTriggerStates.Contains(Name))
	{
		SaveGame->SaveTriggerStates[Name] = bHasSaved;
	}
	else
	{
		SaveGame->SaveTriggerStates.Add(Name, bHasSaved);
	}
}

void ASaveTrigger::HandleSaveDataLoaded(UGP3SaveGame* SaveGame)
{
	FString Name = SaveID;

	if (SaveGame->SaveTriggerStates.Contains(Name))
	{
		bHasSaved = SaveGame->SaveTriggerStates[Name];
	}
}

