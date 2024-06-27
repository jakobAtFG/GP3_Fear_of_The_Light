#include "Enemy.h"
#include "Components/SphereComponent.h"
#include "Components/LightComponent.h"
#include "LightActor.h"

AEnemy::AEnemy()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	VisionSphere = CreateDefaultSubobject<USphereComponent>("VisionSphere");
	VisionSphere->SetupAttachment(Root);
	VisionSphere->SetSphereRadius(900.f);
	VisionSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemy::LightSphereOverlap);
	VisionSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemy::LightSphereEndOverlap);

	PrimaryActorTick.bCanEverTick = true;


}

void AEnemy::BeginPlay()
{

	
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GoAfterLight(DeltaTime);
}

void AEnemy::LightSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (!OtherActor) return;

	TArray<ULightComponent*> lights;

	OtherActor->GetComponents(lights, true);


	if (lights.Num() <= 0) return;

	for (int i = 0; i < lights.Num(); i++)
	{

		if (!TargetLight || lights[i]->Intensity > TargetLight->Intensity) {
			TargetActor = OtherActor;
			UE_LOG(LogTemp, Warning, TEXT("Actor name: %s"), *TargetActor->GetName());
			TargetLight = lights[i];
		}
	}
}

void AEnemy::LightSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}

void AEnemy::FindLight()
{
	
}

void AEnemy::GoAfterLight(float DeltaTime)
{
	if (TargetActor) //TargetActor->GetOwner()
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Found"));

		if (CurrentDistance2 < TotalDistance2)
		{
			FVector Location = GetActorLocation();
			Location += Direction2 * Speed2 * DeltaTime;

			SetActorLocation(Location);
			CurrentDistance2 = (Location - StartLocation2).Size();
		}
		else if (CurrentDistance2 >= TotalDistance2)
		{
			StartLocation2 = GetActorLocation();
			Direction2 = TargetActor->GetActorLocation() - StartLocation2;
			TotalDistance2 = Direction2.Size();

			Direction2 = Direction2.GetSafeNormal();
			CurrentDistance2 = 0.0f;
		}
	}
}
