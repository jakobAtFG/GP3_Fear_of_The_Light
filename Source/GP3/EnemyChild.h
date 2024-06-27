// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Enemy.h"
#include "EnemyChild.generated.h"

/**
 * 
 */
UCLASS()
class GP3_API AEnemyChild : public AEnemy
{
	GENERATED_BODY()
	
public:
	AEnemyChild();

	virtual void Tick(float DeltaTime) override;
};
