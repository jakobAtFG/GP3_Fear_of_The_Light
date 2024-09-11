// Fill out your copyright notice in the Description page of Project Settings.

#include "BatteryComponent.h"
#include "GP3Character.h"

// Sets default values for this component's properties
UBatteryComponent::UBatteryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}

void UBatteryComponent::PauseRechargeWhenMoving(const FInputActionValue& Value)
{
	// UE_LOG(LogTemp, Warning, TEXT("Moving Detection Activated!!!"));
	if (bIsRecharging) bIsRecharging = false;
}

void UBatteryComponent::PauseRechargeWhenLookingAround(const FInputActionValue& Value)
{
	if (OwnerCharacter)
	{
		// UE_LOG(LogTemp, Warning, TEXT("Look Detection start"));
		if (bIsRecharging)
		{
			// UE_LOG(LogTemp, Warning, TEXT("RechargeDirection: %s"), *RechargeDirection.ToString());
			// UE_LOG(LogTemp, Warning, TEXT("OwnerCharacter->ViewForward: %s"), *OwnerCharacter->ViewForward.ToString());
			// UE_LOG(LogTemp, Warning, TEXT("Dot: %f"), RechargeDirection.Dot(OwnerCharacter->ViewForward));
			// UE_LOG(LogTemp, Warning, TEXT("Threshold FMath::DegreesToRadians(%f): %f"), RechargeDirectionRangInDegree,
			// FMath::Cos(
			//  FMath::DegreesToRadians(RechargeDirectionRangInDegree)));

			if (RechargeDirection.Dot(OwnerCharacter->ViewForward) < FMath::Cos(
				FMath::DegreesToRadians(RechargeDirectionRangInDegree)))
			{
				UE_LOG(LogTemp, Warning, TEXT("Look Detection Activated!!!!!!!!!"));
				bIsRecharging = false;
			}
		}
	}
}


// Called when the game starts
void UBatteryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("%f"), FMath::Cos(FMath::DegreesToRadians(RechargeDirectionRangInDegree)));

}


// Called every frame
void UBatteryComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (bIsRecharging)
	{
		Recharge(RechargeRate);
	}

	
	if(LastFramePower == MaxPower && LastFramePower > CurrentPower)
	{
		OnNotFull.Broadcast();
	}
	LastFramePower = CurrentPower;


	// UE_LOG(LogTemp, Warning, TEXT("ViewForward: %s"), *OwnerCharacter->ViewForward.ToString())


	// ...
}

void UBatteryComponent::Recharge(const float Rate)
{
	if (CurrentPower < MaxPower)
	{
		CurrentPower += GetWorld()->DeltaTimeSeconds * Rate;
	}
	else
	{
		CurrentPower = MaxPower;
		bIsRecharging = false;
		OnStopRecharge.Broadcast();
		OnFull.Broadcast();
	}
}

bool UBatteryComponent::Consume(ETool Tool)
{
	if (CurrentPower >= GetWorld()->DeltaTimeSeconds * ConsumeRates[Tool])
	{
		float PowerBeforeConsume = CurrentPower;
		CurrentPower -= GetWorld()->DeltaTimeSeconds * ConsumeRates[Tool];

		
		return true;
	}
	else
	{
		return false;
	}
}

void UBatteryComponent::ToggleRecharge()
{
	bIsRecharging = !bIsRecharging;
	if (bIsRecharging && OwnerCharacter)
	{
		RechargeDirection = OwnerCharacter->ViewForward;
	}
}

bool UBatteryComponent::IsBatteryFull()
{
	return CurrentPower == MaxPower; 
}
