// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyActor.h"
#include "Components/SphereComponent.h"
#include "SwarmEnemyActor.generated.h"

/**
 * 
 */
class AAIController;

UENUM()
enum class ESwarmEnemyState
{
	Passive,
	Attack
};


UCLASS()
class GP3_API ASwarmEnemyActor : public AEnemyActor
{
	GENERATED_BODY()

public:
	ASwarmEnemyActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float Damage = 20;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	bool bIsChasing = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float DistanceFromPlayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float IgnoreLightDistance = 400;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float PlayerDetectRadius = 600.f;


	UFUNCTION(BlueprintImplementableEvent)
	void CheckLineTrace();

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	AActor* CharacterTarget = nullptr;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FHitResult HitResult;

	UFUNCTION(BlueprintImplementableEvent)
	void ChaseCharachterChange();

	UFUNCTION(BlueprintCallable)
	void AttackPlayer();

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PlayerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                         UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                         const FHitResult& SweepResult);
	UFUNCTION()
	void PlayerSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	                            UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	//Current Actor
	FVector Direction;
	FVector StartLocation;
	FVector CurrentDirection;

	float TotalDistance;
	float CurrentDistance;

	//Target
	FRotator TargetRotation;
	FVector TargetDirection;

private:
	//UPROPERTY(VisibleAnywhere)
	//USceneComponent* Root = nullptr;

	UPROPERTY(EditDefaultsOnly)
	AGP3Character* GP3Character = nullptr;

	UPROPERTY(VisibleAnywhere)
	ULightComponent* PreviousLight = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UHealthComponent* PlayerHealthComponent = nullptr;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RotationValue = nullptr;

	UPROPERTY(VisibleAnywhere)
	AAIController* AiController;

	UPROPERTY(VisibleAnywhere)
	ESwarmEnemyState CurrentState = ESwarmEnemyState::Passive;

	TMap<ESwarmEnemyState, void (ASwarmEnemyActor::*)(float DeltaTime)> StateHandlers =
	{
		{ESwarmEnemyState::Passive, &ASwarmEnemyActor::PassiveState},
		{ESwarmEnemyState::Attack, &ASwarmEnemyActor::HuntPlayer}
	};

	UFUNCTION()
	void PassiveState(float DeltaTime);

	UFUNCTION()
	void HuntPlayer(float DeltaTime);

	UFUNCTION()
	void LookForPlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void HandleTargetLightChanged();
};
