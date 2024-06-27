// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LightActor.h"
#include "GP3Character.h"
#include "EmpGun.generated.h"

UCLASS()
class GP3_API AEmpGun : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEmpGun();

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool IsUsingLaserPointerBool = false;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool TurnOfLightBool = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Distance = 1000;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float BatteryDepletion = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULightComponent* TargetLight = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float LastFireTime = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float FireCoolDown = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MissCoolDown = 1;

	UFUNCTION(BlueprintImplementableEvent)
	void PlayEmpSound();

	UFUNCTION(BlueprintImplementableEvent)
	void EmpMisfire();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditInstanceOnly)
	FHitResult HitResultEmp;

	UPROPERTY(EditDefaultsOnly)
	TArray<ULightComponent*> Lights;

	UPROPERTY(EditDefaultsOnly)
	AGP3Character* GP3Character = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UToolManager* ToolManager = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UBatteryComponent* BatteryComponent = nullptr;

	UFUNCTION()
	void Shoot(bool bIsToggledOn, ETool Tool);

	UFUNCTION()
	void BlueprintLineTraceFromPlayer();

	UFUNCTION()
	void Misfire();
};
