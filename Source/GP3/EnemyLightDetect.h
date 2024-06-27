#pragma once
#include "Components/ActorComponent.h"
#include "LightActor.h"
#include "EnemyLightDetect.generated.h"


USTRUCT()
struct FLightSources
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	AActor* FLightSource = nullptr;
};

UCLASS(meta = (BlueprintSpawnableComponent))
class UEnemyLightDetect : public UActorComponent
{
	GENERATED_BODY()

public:
	UEnemyLightDetect();



	UPROPERTY(BlueprintReadWrite)
	TArray<float> LightLevel;

	int NrOfControlPoint;
	AActor* TargetActor = nullptr;
	ULightComponent* TargetLight = nullptr;
	ALightActor* LightTargetActor;
	//ULightComponent* LightTargetComponent;


	
private:

	UPROPERTY(EditInstanceOnly)
	TArray<FLightSources> LightSources;

	UFUNCTION()
	void FindLight();

	UFUNCTION()
	void GoAfterLight();
	
};
