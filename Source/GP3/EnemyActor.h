// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Actor.h"
#include "EnemyLightDetect.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameFramework/Character.h"
#include "EnemyActor.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnTargetLightChanged);

class UGP3SaveGame;

UCLASS()
class GP3_API AEnemyActor : public ACharacter
{
	GENERATED_BODY()

public:
	AEnemyActor();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "EnemySettings")
	float LightDetectBrightness = 2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySettings")
	ULightComponent* TargetLight = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySettings")
	TArray<ULightComponent*> AvailableLightsTArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "EnemySettings")
	TArray<ALightActor*> AvailableLightActorsTArray;

	UPROPERTY(EditAnywhere)
	USphereComponent* VisionSphere;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

protected:
	virtual void BeginPlay() override;


	UFUNCTION()
	void LightSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* LightOtherActor,
	                        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
	                        const FHitResult& SweepResult);

	UFUNCTION()
	void LightSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* LightOtherActor,
	                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void FindLight(AActor* LightOtherActor);

	UFUNCTION()
	void RemoveLight(AActor* LightOtherActor);

	UFUNCTION()
	void SortAndAssignLight();

	UFUNCTION()
	void LookForLight();

	UFUNCTION()
	void BeginPlayAddLights();

	UPROPERTY(BlueprintAssignable)
	FOnTargetLightChanged OnTargetLightChanged;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	FHitResult HitResultLights;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<AActor*> StartActorArray;

private:
	UPROPERTY()
	TArray<ULightComponent*> Lights;

	UPROPERTY()
	ULightComponent* PreviousTargetLight = nullptr;

};
