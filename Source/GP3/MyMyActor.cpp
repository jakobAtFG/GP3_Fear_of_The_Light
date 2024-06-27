// Fill out your copyright notice in the Description page of Project Settings.


#include "MyMyActor.h"

AMyMyActor::AMyMyActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AMyMyActor::BeginPlay()
{
	Super::BeginPlay();

}

void AMyMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("Update test"));

}
