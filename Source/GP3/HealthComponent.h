// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "HealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHealthDeathSignature);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnDamage, float, healthPercent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHeal, float, healthPercent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealthComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(UIMin = "0.0", UIMax = "500.0"))
	float MaximumHealth = 100.f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurrentHealth = 0.f;

	UFUNCTION(BlueprintCallable)
	void TakeDamage(float damage);

	UFUNCTION(BlueprintCallable)
	void Heal(float healAmount);
	
	UPROPERTY(BlueprintAssignable)
	FHealthDeathSignature OnDeath;
	
	UPROPERTY(BlueprintAssignable)
	FOnDamage OnDamage;

	UPROPERTY(BlueprintAssignable)
	FOnHeal OnHeal;
};
