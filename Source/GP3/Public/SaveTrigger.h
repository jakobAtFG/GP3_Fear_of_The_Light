// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SaveTrigger.generated.h"

class UGP3SaveGame;
class UBoxComponent;

UCLASS()
class GP3_API ASaveTrigger : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASaveTrigger();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSaveOnlyOnce = true;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* BoxComponent;
	
	UFUNCTION()
	void OnOverlapBegin(
		class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
		);

	UFUNCTION()
	void HandleSaveRequested(UGP3SaveGame* SaveGame);

	UFUNCTION()
	void HandleSaveDataLoaded(UGP3SaveGame* SaveGame);

private:

	bool bHasSaved = false;
};
