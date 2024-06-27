#include "SwarmEnemyActor.h"

#include "GP3Character.h"
#include "Components/SphereComponent.h"
#include "Components/InterpToMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "Kismet/GameplayStatics.h"


#include "Components/LightComponent.h"

ASwarmEnemyActor::ASwarmEnemyActor()
{
	RotationValue = CreateDefaultSubobject<USceneComponent>("YawValue");
	RotationValue->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>("Sphere");
	Sphere->SetupAttachment(RootComponent);
	Sphere->SetSphereRadius(600.f);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ASwarmEnemyActor::PlayerSphereOverlap);
	Sphere->OnComponentEndOverlap.AddDynamic(this, &ASwarmEnemyActor::PlayerSphereEndOverlap);

	PrimaryActorTick.bCanEverTick = true;
}


void ASwarmEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	AiController = Cast<AAIController>(GetController());

	//Add Dynamic so we can use the function in parent script.
	OnTargetLightChanged.AddDynamic(this, &ASwarmEnemyActor::HandleTargetLightChanged);

	//Assign the player so we can access it health component.
	GP3Character = Cast<AGP3Character>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());
	if (GP3Character)
	{
		PlayerHealthComponent = GP3Character->GetComponentByClass<UHealthComponent>();
	}
}


void ASwarmEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Plays the current state we are in.
	(this->*StateHandlers[CurrentState])(DeltaTime);
}

void ASwarmEnemyActor::PlayerSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                           const FHitResult& SweepResult)
{
	//If the overlapping actor is a AGP3Character which is a then assign is as the Character we target.
	if (OtherActor->IsA<AGP3Character>())
	{
		CharacterTarget = OtherActor;
	}
}

void ASwarmEnemyActor::PlayerSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	//Remove Character Target if it leave the sphere.
	if (OtherActor == CharacterTarget)
	{
		CharacterTarget = nullptr;
	}
}

void ASwarmEnemyActor::PassiveState(float DeltaTime)
{
	//If we get a Target Character then we stop going where we are going and switch to the Attack State.
	if (CharacterTarget)
	{
		DistanceFromPlayer = GetDistanceTo(CharacterTarget);
		float ClampedIgnoreLightDistance =
			FMath::Clamp(IgnoreLightDistance, 0.0f, Sphere->GetUnscaledSphereRadius() - 100);

		//If the player is either lit up or within a certain distance from us then continue else we return here. 
		if (Cast<AGP3Character>(CharacterTarget)->PlayerIsLitUp == true || DistanceFromPlayer <
			ClampedIgnoreLightDistance)
		{
			AiController->StopMovement();
			CurrentState = ESwarmEnemyState::Attack;
			return;
		}
	}

	if (bIsChasing)
	{
		//If the Swarm was previously going after the player.

		bIsChasing = false;

		//Changes the look of the swarm, calm them down.
		ChaseCharachterChange();

		//If we did not have a light target before we started going after the player then return.
		if (!PreviousLight) return;

		//Move towards the previous light we had.
		ALightActor* PreviousLightLightActorTarget = Cast<ALightActor>(PreviousLight->GetOwner());
		AiController->MoveToLocation(PreviousLightLightActorTarget->LightDownLocation);
	}

	//We play the sort and assign function in the parent script.
	SortAndAssignLight();
}

void ASwarmEnemyActor::HuntPlayer(float DeltaTime)
{
	if (!CharacterTarget)
	{
		//If the swarm losses the player then stop moving and switch to the passive state.
		AiController->StopMovement();
		CurrentState = ESwarmEnemyState::Passive;
		return;
	}

	if (bIsChasing == false)
	{
		if (TargetLight)
		{
			//If we had a target light then we save it so we move towards it if we later switch to passive state.
			PreviousLight = TargetLight;
		}

		//If we are in the light or we are close enough we do a line trace. And check if there is something blocking the view between the player and the swarm.
		LookForPlayer();

		if (!HitResult.bBlockingHit)
		{
			//If there were nothing blocking the line trace from the swarm to he player than we start chasing them.
			bIsChasing = true;

			//Changes the look of the swarm /makes them angry when attacking.
			ChaseCharachterChange();

			//Move towards the player.
			AiController->MoveToActor(CharacterTarget);
		}
	}
}


void ASwarmEnemyActor::LookForPlayer()
{
	//Line trace from the swarm to the player location.
	AGP3Character* GP3CharacterTarget = Cast<AGP3Character>(CharacterTarget);

	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);

	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), CharacterTarget->GetActorLocation(),
	                                     ECC_Visibility, CollisionQueryParams);
}

void ASwarmEnemyActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void ASwarmEnemyActor::AttackPlayer()
{
	if (PlayerHealthComponent != nullptr)
	{
		PlayerHealthComponent->TakeDamage(Damage);
	}
}

void ASwarmEnemyActor::HandleTargetLightChanged()
{
	//Get the target light actor and access its down location and move towards it, used in the EnemyActor script.
	ALightActor* LightActorTarget = Cast<ALightActor>(TargetLight->GetOwner());
	AiController->MoveToLocation(LightActorTarget->LightDownLocation);
}
