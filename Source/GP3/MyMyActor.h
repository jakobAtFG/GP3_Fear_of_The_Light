// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyActor.h"
#include "MyMyActor.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API AMyMyActor : public AMyActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMyMyActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
