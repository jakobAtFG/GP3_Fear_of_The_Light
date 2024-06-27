// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SoundDetectComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GP3_API USoundDetectComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USoundDetectComponent();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SoundSettings")
	USoundBase* SoundToPlay = nullptr;

	UFUNCTION()
	void PlaySoundAtLocation
	(
		const UObject* WorldContextObject,
		USoundBase* Sound,
		FVector Location,
		FRotator Rotation,
		float VolumeMultiplier,
		float PitchMultiplier,
		float StartTime,
		class USoundAttenuation* AttenuationSettings,
		USoundConcurrency* ConcurrencySettings,
		AActor* OwningActor
	);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
