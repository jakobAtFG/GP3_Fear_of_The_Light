#pragma once
#include "CoreMinimal.h"
#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "SwarmMoths.generated.h"

class UEnemyLightDetect;


UCLASS()
class GP3_API ASwarmMoths : public AEnemy
{
	GENERATED_BODY()

public:
	ASwarmMoths();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float Speed = 8;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void PlayerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void PlayerSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

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

	UPROPERTY(VisibleAnywhere)
	USphereComponent* Sphere = nullptr;


	UPROPERTY(EditInstanceOnly)
	AActor* CharacterTarget = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* RotationValue = nullptr;

	UPROPERTY(VisibleAnywhere)
	UEnemyLightDetect* EnemyLightDetectComponent = nullptr;

	UFUNCTION()
	void PassiveState(float DeltaTime);

	UFUNCTION()
	void AttackPlayer(float DeltaTime);

};