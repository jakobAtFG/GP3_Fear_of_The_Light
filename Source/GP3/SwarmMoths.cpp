#include "SwarmMoths.h"
#include "Components/SphereComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "LightActor.h"
#include "Enemy.h"




ASwarmMoths::ASwarmMoths()
{
	RotationValue = CreateDefaultSubobject<USceneComponent>("YawValue");
	RotationValue->SetupAttachment(Root);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(Root);
	Sphere->SetSphereRadius(600.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASwarmMoths::PlayerSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ASwarmMoths::PlayerSphereEndOverlap);
	

	EnemyLightDetectComponent = CreateDefaultSubobject<UEnemyLightDetect>("EnemyLightDetect");


	PrimaryActorTick.bCanEverTick = true;
}



void ASwarmMoths::BeginPlay()
{
	Super::BeginPlay();

}

void ASwarmMoths::PlayerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA<ACharacter>())
	{
		CharacterTarget = OtherActor;
	}
}

void ASwarmMoths::PlayerSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == CharacterTarget)
	{
		CharacterTarget = nullptr;
	}
}

void ASwarmMoths::PassiveState(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("PassiveState"));

	if (TargetActor) //TargetActor->GetOwner()
	{
		UE_LOG(LogTemp, Warning, TEXT("Target Found"));

		if (CurrentDistance < TotalDistance)
		{
			FVector Location = GetActorLocation();
			Location += Direction * Speed * DeltaTime;

			SetActorLocation(Location);
			CurrentDistance = (Location - StartLocation).Size();
		}
		else if (CurrentDistance >= TotalDistance)
		{
			StartLocation = GetActorLocation();
			Direction = TargetActor->GetOwner()->GetActorLocation() - StartLocation;
			TotalDistance = Direction.Size();

			Direction = Direction.GetSafeNormal();
			CurrentDistance = 0.0f;
		}
	}
	
}

void ASwarmMoths::AttackPlayer(float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("CharacterTarget Found"));
	CurrentDirection = RotationValue->GetForwardVector();
	TargetDirection = CharacterTarget->GetActorLocation() - RotationValue->GetComponentLocation();
	TargetDirection.Normalize();

	CurrentDirection = UKismetMathLibrary::Vector_SlerpVectorToDirection(CurrentDirection, TargetDirection, 5.f * DeltaTime);

	TargetRotation = UKismetMathLibrary::MakeRotFromX(CurrentDirection);
	RotationValue->SetRelativeRotation(FRotator(TargetRotation.Pitch, TargetRotation.Yaw, 0.f));
	
	
}

void ASwarmMoths::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UE_LOG(LogTemp, Warning, TEXT("!!!!!!!!!!!!!!!!!!!!!!!!"));


	if (!CharacterTarget)
	{
		PassiveState(DeltaTime);
		UE_LOG(LogTemp, Warning, TEXT("!CharacterTarget"));

	}
	else if (CharacterTarget)
	{
		AttackPlayer(DeltaTime);
	}
	
}
