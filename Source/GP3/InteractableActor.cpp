// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

#include "GP3.h"
#include "GP3Character.h"

// Sets default values
AInteractableActor::AInteractableActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	LineTraceDetectionRange = CreateDefaultSubobject<USphereComponent>("Line Detection");
	LineTraceDetectionRange->SetSphereRadius(64.f);
	LineTraceDetectionRange->SetupAttachment(RootComponent);
	LineTraceDetectionRange->OnComponentBeginOverlap.AddDynamic(this, &AInteractableActor::HandleStartingOverlap);
	LineTraceDetectionRange->OnComponentEndOverlap.AddDynamic(this, &AInteractableActor::HandleEndingOverlap);

	LineTraceDetectionRange->SetCollisionResponseToChannel(ECC_InteractionDetection,ECR_Overlap);
	LineTraceDetectionRange->SetCollisionResponseToChannel(ECC_Interaction,ECR_Block);
	LineTraceDetectionRange->SetCollisionObjectType(ECC_InteractionDetection);
}

// Called when the game starts or when spawned
void AInteractableActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AInteractableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AInteractableActor::Interaction()
{
	UE_LOG(LogTemp,Warning, TEXT("%s is interacted by player"), *GetName());
	OnInteract.Broadcast();
}

void AInteractableActor::HandleStartingOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                               UPrimitiveComponent* OtherComponent, int32 OtherBodyIndex,
                                               bool bFromSweep, const FHitResult& Hit)
{
	// if (OtherActor->IsA(AGP3Character::StaticClass()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("%s is in range of %s"), *OtherActor->GetName(), *GetName());
	// 	bIsInteractable = true;
	// }
}

void AInteractableActor::HandleEndingOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                             UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// if (OtherActor->IsA(AGP3Character::StaticClass()))
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("%s is out of range of %s"), *OtherActor->GetName(), *GetName());
	// 	bIsInteractable = false;
	// }
}
