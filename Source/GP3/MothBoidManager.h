// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "MothBoidManager.generated.h"


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class GP3_API UMothBoidManager : public UActorComponent
{
  	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BoidActor;

	// Sets default values for this component's properties
	UMothBoidManager();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:

	TArray<AActor*> Boids;

	void SpawnBoids(int32 NumberOfBoids);

};
