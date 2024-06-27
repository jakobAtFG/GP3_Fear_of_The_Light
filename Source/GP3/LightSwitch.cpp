// Fill out your copyright notice in the Description page of Project Settings.


#include "LightSwitch.h"
#include "Kismet/GameplayStatics.h"
#include "Net/Iris/ReplicationSystem/ReplicationSystemUtil.h"


ALightSwitch::ALightSwitch()
{	
	PrimaryActorTick.bCanEverTick = true;
}

void ALightSwitch::BeginPlay() {
	Super::BeginPlay();
	// for (int i = 0; i < LightArray.Num(); i++)
	// {
	// 	if (LightSwitched == false)
	// 	{
	// 		LightArray[i]->SetActorHiddenInGame(true);
	//
	// 		UE_LOG(LogTemp, Warning, TEXT("Lights ON"));
	// 	}
	// 	else if (LightSwitched == true)
	// 	{
	// 		LightArray[i]->SetActorHiddenInGame(false);
	//
	// 		UE_LOG(LogTemp, Warning, TEXT("Lights OFF"));
	// 	}
	// }
}

void ALightSwitch::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	// LightSwith();
}


void ALightSwitch::LightSwitch()
{

		LightSwitched = !LightSwitched;
		
		for (int i = 0; i < LightArray.Num(); i++)
		{
			if (LightSwitched == false)
			{
				LightArray[i]->SetActorHiddenInGame(true);

				UE_LOG(LogTemp, Warning, TEXT("Lights ON"));
			}
			else if (LightSwitched == true)
			{
				LightArray[i]->SetActorHiddenInGame(false);

				UE_LOG(LogTemp, Warning, TEXT("Lights OFF"));
			}

		}

}
	
