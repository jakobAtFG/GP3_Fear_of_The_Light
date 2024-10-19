// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ToolManager.h"
#include "Components/ActorComponent.h"
#include "BatteryComponent.generated.h"

class AGP3Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBatterySignature);
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GP3_API UBatteryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBatteryComponent();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGP3Character* OwnerCharacter = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsRecharging = false;

	bool bIsFull = false;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RechargeRate = 5.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentPower = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxPower = 100.f;
	
	float LastFramePower =  0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalConsumedPower = 0.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector RechargeDirection = FVector(0.f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RechargeDirectionRangInDegree = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<ETool, float> ConsumeRates
	{
		{ETool::NONE, 0.f},
		{ETool::RADAR, 0.f},
		{ETool::HEAT_VISION, 0.f},
		{ETool::EMP, 0.f}
	};

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable)
	FBatterySignature OnStartRecharge;

	UPROPERTY(BlueprintAssignable)
	FBatterySignature OnStopRecharge;
	
	UPROPERTY(BlueprintAssignable)
	FBatterySignature OnFull;

	UPROPERTY(BlueprintAssignable)
	FBatterySignature OnNotFull;;

	UFUNCTION()
	void Recharge(float Rate);

	UFUNCTION(BlueprintCallable)
	bool Consume(ETool Tool);

	UFUNCTION(BlueprintCallable)
	void ToggleRecharge();

	UFUNCTION(BlueprintCallable)
	bool IsBatteryFull();

	UFUNCTION()
	void PauseRechargeWhenMoving(const FInputActionValue& Value);

	UFUNCTION()
	void PauseRechargeWhenLookingAround(const FInputActionValue& Value);
};
