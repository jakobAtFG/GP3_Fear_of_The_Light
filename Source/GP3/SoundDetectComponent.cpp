// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundDetectComponent.h"

// Sets default values for this component's properties
USoundDetectComponent::USoundDetectComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}



void USoundDetectComponent::PlaySoundAtLocation(const UObject* WorldContextObject, USoundBase* Sound, FVector Location, FRotator Rotation, float VolumeMultiplier, float PitchMultiplier, float StartTime, USoundAttenuation* AttenuationSettings, USoundConcurrency* ConcurrencySettings, AActor* OwningActor)
{
	Sound = SoundToPlay;


}

// Called when the game starts
void USoundDetectComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


// Called every frame
void USoundDetectComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
