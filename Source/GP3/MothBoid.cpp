// Fill out your copyright notice in the Description page of Project Settings.
//MothBoid.cpp

#include "MothBoid.h"


// Sets default values for this component's properties
UMothBoid::UMothBoid()
{
//	GetOwner()->PrimaryActorTick.bCanEverTick = true;
}


// Called when the game starts
void UMothBoid::BeginPlay()
{
	Super::BeginPlay();
    UE_LOG(LogTemp, Warning, TEXT("The MothBoid c++ class script works"));

	 Position = GetOwner()->GetActorLocation();
	 Velocity = FVector::ZeroVector;
}


// Called every frame
void UMothBoid::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	 ApplyRules();
	 Position += Velocity * DeltaTime;
	 GetOwner()->SetActorLocation(Position);
}


 FVector UMothBoid::CalculateSeparation()
 {
 	return FVector::ZeroVector;
 }

 FVector UMothBoid::CalculateAlignment()
 {
 	return FVector::ZeroVector;
 }

 FVector UMothBoid::CalculateCohesion()
 {
 	return FVector::ZeroVector;
 }

 void UMothBoid::ApplyRules()
 {
 	FVector Separation = CalculateSeparation();
 	FVector Alignment = CalculateAlignment();
 	FVector Cohesion = CalculateCohesion();

 	Velocity += Separation + Alignment + Cohesion;
 	Velocity = Velocity.GetClampedToMaxSize(MothMaxSpeed);
 }

