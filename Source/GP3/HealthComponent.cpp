// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaximumHealth;

	// ...
	
}


// Called every frame
void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UHealthComponent::TakeDamage(float damage)
{
	CurrentHealth -= damage;
	OnDamage.Broadcast(CurrentHealth/MaximumHealth);
	if(CurrentHealth <= 0.f)
	{
		CurrentHealth = 0;
		OnDeath.Broadcast();
	}
}

void UHealthComponent::Heal(float healAmount)
{
	CurrentHealth += healAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, 100.0f);
	OnHeal.Broadcast(CurrentHealth / MaximumHealth);
}


