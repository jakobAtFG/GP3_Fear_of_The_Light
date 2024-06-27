// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Radar.generated.h"

class UWidget;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_API URadar : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URadar();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float Range = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float Angle = 90.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float PluseSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float FadeSpeed = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float MovementMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float BaseBlipIntensity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	float BatteryDrainRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	TSubclassOf<UUserWidget> RadarWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Radar")
	TSubclassOf<UUserWidget> BlipWidgetClass;

	UPROPERTY(BlueprintReadWrite, Category = "Radar")
	UUserWidget* RadarWidget = nullptr;

	UPROPERTY(BlueprintReadWrite, Category = "Radar")
	UWidget* RadarImage = nullptr;

private:
	TArray<AActor> DetectedObjects;

	void PlaceBlip(AActor* Actor);
 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
