#pragma once
#include "Components/SpotLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GP3Character.h"
#include "LightActor.generated.h"

class AGP3Character;

UCLASS()
class ALightActor : public AActor
{
	GENERATED_BODY()

public:
	ALightActor();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root = nullptr;

	UPROPERTY(EditAnywhere, Category = "LightSettings")
	bool IsTurnedOfEmp = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSettings")
	float TurnOnDelay = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LightSettings")
	FVector LightDownLocation;

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOffLight();

	UFUNCTION(BlueprintImplementableEvent)
	void TurnOnLight();

	UFUNCTION()
	void StartLineTrace();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	ULightComponent* TargetLight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULightComponent* LightActorsTargetLight = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USphereComponent* LightSphere;

protected:


private:
	virtual void BeginPlay() override;

	UFUNCTION()
	void TurnLightBackOn();

	UPROPERTY(EditDefaultsOnly)
	TArray<ULightComponent*> Lights;

	FTimerHandle MemberTimerHandle;

	bool bDoneOnce = false;

	int32 CallTracker;
};
