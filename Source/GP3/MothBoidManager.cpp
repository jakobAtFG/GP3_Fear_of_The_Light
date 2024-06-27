// Fill out your copyright notice in the Description page of Project Settings.
//MothBoidManager.cpp

#include "MothBoidManager.h"
#include "MothBoid.h"

UMothBoidManager::UMothBoidManager()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UMothBoidManager::BeginPlay()
{
	Super::BeginPlay();

    UE_LOG(LogTemp, Warning, TEXT("The MothBoidManager c++ class script works"));
    SpawnBoids(50);


}

void UMothBoidManager::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{

    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    for (AActor* Boid : Boids)
     {
         if (Boid)
         {

             UMothBoid* BoidComponent = Boid->FindComponentByClass<UMothBoid>();
             if (BoidComponent)
             {

                 BoidComponent->TickComponent(DeltaTime, TickType, ThisTickFunction);
             }
         }
     }
}

 void UMothBoidManager::SpawnBoids(int32 NumberOfBoids) 
 {

     UWorld* World = GetWorld();

     
 	for (int32 i = 0; i < NumberOfBoids; ++i)
 	{

         // Generate a random location within specified bounds
         FVector SpawnLocation = FVector(
             FMath::RandRange(-1000, 1000),  // Random X position
             FMath::RandRange(-1000, 1000),  // Random Y position
             0);                            // Fixed Z position


         FRotator SpawnRotation = FRotator::ZeroRotator;
         FActorSpawnParameters SpawnParams;
         SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

         // Spawn the boid actor in the world
         AActor* NewBoidActor = World->SpawnActor<AActor>(BoidActor, SpawnLocation, SpawnRotation, SpawnParams);
        
         // Check if the boid was successfully spawned
         if (NewBoidActor)
         {
             // Add the spawned boid to the array for management
             Boids.Add(NewBoidActor);
             UE_LOG(LogTemp, Warning, TEXT("Moth number %d has been spawned"), i);
         }


 	}

 }
