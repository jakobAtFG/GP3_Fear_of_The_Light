// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyChild.h"

AEnemyChild::AEnemyChild()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemyChild::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogTemp, Warning, TEXT("Update test"));

}
