// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "InteractableActor.generated.h"
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInteractableSignature);
UCLASS()
class GP3_API AInteractableActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* LineTraceDetectionRange = nullptr;

	UPROPERTY(VisibleAnywhere)
	bool bIsInteractable = false;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void ShowInfoOnScreen();

	UFUNCTION(BlueprintImplementableEvent)
	void CloseInfoOnScreen();

	UFUNCTION(BlueprintCallable)
	virtual void Interaction();

	UPROPERTY(BlueprintAssignable)
	FInteractableSignature OnInteract;

	UFUNCTION()
	void HandleStartingOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComponent,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& Hit
	);

	UFUNCTION()
	void HandleEndingOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex
	);
};
