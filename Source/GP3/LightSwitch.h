// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "LightSwitch.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API ALightSwitch : public AInteractableActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	bool LightSwitched = true;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	//int32 LightComponentArrayLenght;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom")
	TArray<AActor*> LightArray;

	ALightSwitch();
	virtual void BeginPlay() override;
	void Tick(float DeltaTime) override;

protected:
	UFUNCTION(BlueprintCallable)
	void LightSwitch();
};
