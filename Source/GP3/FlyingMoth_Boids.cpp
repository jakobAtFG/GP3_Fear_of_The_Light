// Fill out your copyright notice in the Description page of Project Settings.


#include "FlyingMoth_Boids.h"

// Sets default values
AFlyingMoth_Boids::AFlyingMoth_Boids()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFlyingMoth_Boids::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFlyingMoth_Boids::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

