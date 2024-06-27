#pragma once

#include "EnemyLightDetect.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"

class UEnemyLightDetect;


UCLASS()
class GP3_API AEnemy : public AActor
{
	GENERATED_BODY()

public:
	AEnemy();
	virtual void BeginPlay();

	virtual void Tick(float DeltaTime);


	UPROPERTY(EditAnywhere)
	USphereComponent* VisionSphere;

	UPROPERTY(BlueprintReadWrite)
	TArray<float> LightLevel;

	int NrOfControlPoint;

	UPROPERTY(VisibleAnywhere)
	AActor* TargetActor = nullptr;


	ULightComponent* TargetLight = nullptr;
	ALightActor* LightTargetActor;


	//ULightComponent* LightTargetComponent;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

private:
	
	UFUNCTION()
	void LightSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void LightSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditInstanceOnly)
	TArray<FLightSources> LightSources;

	UFUNCTION()
	void FindLight();

	UFUNCTION()
	void GoAfterLight(float DeltaTime);



	FVector Direction2;
	FVector StartLocation2;
	FVector CurrentDirection2;
	float TotalDistance2;
	float CurrentDistance2;
	FRotator TargetRotation2;
	FVector TargetDirection2;
	float Speed2 = 8;
};