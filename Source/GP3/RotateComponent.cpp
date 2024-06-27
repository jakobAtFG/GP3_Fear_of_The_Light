// Fill out your copyright notice in the Description page of Project Settings.

#include "RotateComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values for this component's properties
URotateComponent::URotateComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URotateComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URotateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//Rotation = RotateSpeed * DeltaTime;
	//RotationValue->SetRelativeRotation(FRotator(Rotation, 0, 0));

	//AActor* RotateActor;

	
		//RotateActor = RotateActorArray[i];
	GetOwner()->AddActorLocalRotation(FRotator(RotateSpeedX, RotateSpeedY, RotateSpeedZ));
		
	//RotateActor.AddActorLocalRotation(FRotator(RotateSpeed, RotateSpeed, RotateSpeed));
	
}

