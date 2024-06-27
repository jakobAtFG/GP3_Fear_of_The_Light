// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyActor.h"

#include "Components/SphereComponent.h"
#include "Components/LightComponent.h"
#include "LightActor.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AEnemyActor::AEnemyActor()
{
	VisionSphere = CreateDefaultSubobject<USphereComponent>("VisionSphere");
	VisionSphere->SetupAttachment(RootComponent);
	VisionSphere->SetSphereRadius(900.f);
	VisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemyActor::LightSphereOverlap);
	VisionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemyActor::LightSphereEndOverlap);

	PrimaryActorTick.bCanEverTick = true;
}

void AEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
	BeginPlayAddLights();
}

void AEnemyActor::LightSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* LightOtherActor,
                                     UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                     const FHitResult& SweepResult)
{
	FindLight(LightOtherActor);
}

void AEnemyActor::LightSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* LightOtherActor,
                                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	RemoveLight(LightOtherActor);
}

void AEnemyActor::FindLight(AActor* LightOtherActor)
{
	//Check if the actor that overlap is a light actor. 
	if (!LightOtherActor->IsA<ALightActor>()) return;

	//If it was than add it to the ALightActor array.
	ALightActor* ActorLight = Cast<ALightActor>(LightOtherActor);
	AvailableLightActorsTArray.Add(ActorLight);

	//Get the light component on the light actor and add it to the Available Lights TArray.
	LightOtherActor->GetComponents<ULightComponent>(Lights, true);
	AvailableLightsTArray.Append(Lights);
}

void AEnemyActor::RemoveLight(AActor* LightOtherActor)
{
	//Check if the leaving actor is a light actor.
	if (!LightOtherActor->IsA<ALightActor>()) return;

	//Remove the LightActor from the available LightActors TArray. 
	AvailableLightActorsTArray.Remove(Cast<ALightActor>(LightOtherActor));

	//Get the light actor light components and remove them from our light array.
	LightOtherActor->GetComponents<ULightComponent>(Lights, true);
	for (ULightComponent* light : Lights)
	{
		AvailableLightsTArray.Remove(light);
	}
}

void AEnemyActor::SortAndAssignLight()
{
	//If no light has entered then return
	if (AvailableLightsTArray.Num() == 0) return; 
	

	//Filter all the lights and save the lights that are visible and have a higher intensity than 0 and add it no a new array.
	TArray<ULightComponent*> FilteredAvailableLightsTArray = AvailableLightsTArray.FilterByPredicate(
		[](ULightComponent* Light)
		{
			return Light->Intensity > 0 && Light->IsVisible();
		});

	//Check if the array is empty and if so then dont continue.
	if (FilteredAvailableLightsTArray.Num() == 0) return;

	//Sort the array so the light with the highest intensity is the first in the array.
	FilteredAvailableLightsTArray.Sort([](ULightComponent& A, ULightComponent& B)
	{
		return A.Intensity >= B.Intensity;
	});

	//Assign the target light as the first array in the array of lights.
	TargetLight = FilteredAvailableLightsTArray[0];

	//Check if our target light is the same as the previous light.
	if (PreviousTargetLight != TargetLight)
	{
		//Line trace from the enemy to the target light location.
		LookForLight();

		//If we something is blocking then we stop.
		if (HitResultLights.bBlockingHit) return;

		//If we have a line of sight than we assign it as our previous light.
		PreviousTargetLight = TargetLight;

		//Use dynamic function in the swarm script that makes ut move towards the light.
		OnTargetLightChanged.Broadcast();
	}
}

void AEnemyActor::LookForLight()
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResultLights, GetActorLocation(),
	                                     TargetLight->GetOwner()->GetActorLocation(),
	                                     ECC_WorldDynamic, CollisionQueryParams);
}

void AEnemyActor::BeginPlayAddLights()
{
	//Get the overlapping actors and check if it there is any light actors and add the light components on them to the lights array.
	VisionSphere->GetOverlappingActors(StartActorArray, ALightActor::StaticClass());
	for (int i = 0; i < StartActorArray.Num(); i++)
	{
		StartActorArray[i]->GetComponents<ULightComponent>(Lights, true);
		if (Lights.Num() > 0)
		{
			AvailableLightActorsTArray.Add(Cast<ALightActor>(StartActorArray[i]));
		}
		AvailableLightsTArray.Append(Lights);

		//ALightActor* ActorLight = Cast<ALightActor>(StartActorArray[i]);
		//AvailableLightsTArray.Add(ActorLight->LightActorsTargetLight);
	}
}
