#include "LightActor.h"

#include "GP3.h"
#include "SwarmEnemyActor.h"


#include "Components/SpotLightComponent.h"

ALightActor::ALightActor()
{
	Root = CreateDefaultSubobject<USceneComponent>("Root");
	RootComponent = Root;

	LightSphere = CreateDefaultSubobject<USphereComponent>("VisionSphere");
	LightSphere->SetupAttachment(Root);
	LightSphere->SetSphereRadius(50.f);
	//LightSphere->SetCollisionProfileName(TEXT("Pawn"));
	LightSphere->SetCollisionObjectType(ECC_EMP);

	LightSphere->SetVisibility(false);

	PrimaryActorTick.bCanEverTick = true;
}


void ALightActor::BeginPlay()
{
	Super::BeginPlay();

	//Get the light component to the light target.
	GetComponents(Lights, true);
	LightActorsTargetLight = Lights[0];

	//Set the call tracker to the timer time.
	CallTracker = TurnOnDelay;

	//Do line trace down and save the position so the enemy can move to it.
	StartLineTrace();
}

void ALightActor::StartLineTrace()
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, GetActorLocation(), GetActorLocation() + FVector::DownVector * 1000,
	                                     ECC_WorldStatic, CollisionQueryParams);

	if (HitResult.bBlockingHit)
	{
		//If we hit something assign the impact position as our down location.
		LightDownLocation = HitResult.ImpactPoint;
	}
}

void ALightActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//If the light shoot by the emp then do this.
	if (IsTurnedOfEmp == true && bDoneOnce == false)
	{
		bDoneOnce = true;

		//Get the light component.
		GetComponents(Lights, true);
		TargetLight = Lights[0];

		//Trigger blueprint function to turn of the light component.
		TurnOffLight();
		
		//TargetLight->SetVisibility(true);

		//Start timer on the turn light back on function.
		GetWorldTimerManager().SetTimer(MemberTimerHandle, this, &ALightActor::TurnLightBackOn, 1.0f, true, 0.5f);
	}
}


void ALightActor::TurnLightBackOn()
{
	//Lower the timer and if its get to 0 or bellow trigger something.
	--CallTracker;
	if (CallTracker <= 0)
	{
		//Clear the timer and reset it.
		GetWorldTimerManager().ClearTimer(MemberTimerHandle);
		CallTracker = TurnOnDelay;
		bDoneOnce = false;

		//Make it able to be shot by the emp again.		
		IsTurnedOfEmp = false;

		//Trigger blueprint function to turn the light component back on.
		TurnOnLight();
		
		//Lights[0]->SetVisibility(true);
	}
}
