// Fill out your copyright notice in the Description page of Project Settings.

#include "EmpGun.h"

#include "BatteryComponent.h"
#include "GP3Character.h"
#include "GP3.h"
#include "ToolManager.h"
#include "BatteryComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEmpGun::AEmpGun()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AEmpGun::BeginPlay()
{
	Super::BeginPlay();

	GP3Character = Cast<AGP3Character>(UGameplayStatics::GetPlayerController(GetWorld(), 0)->GetCharacter());

	if (GP3Character)
	{
		ToolManager = GP3Character->GetComponentByClass<UToolManager>();
		BatteryComponent = GP3Character->GetComponentByClass<UBatteryComponent>();
	}

	if (ToolManager)
	{
		ToolManager->OnToolToggled.AddDynamic(this, &AEmpGun::Shoot);
	}

	LastFireTime = 0;
}

// Called every frame
void AEmpGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	BlueprintLineTraceFromPlayer();
}

void AEmpGun::Shoot(bool bIsToggledOn, ETool Tool)
{
	//If our current tool is not the emp, if we dont have a target light and if the light is not visible/ intensity is 0, if any then we stop.
	if (Tool != ETool::EMP) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No EMP Equipped"));
		return;
	}

	//If gun is in cooldown
	if (!TargetLight)
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No target light"));
		Misfire();
		return;
	}
	if ((!Lights[0]->IsVisible()) || (Lights[0]->Intensity == 0))
	{
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Target light is already turned off"));
		Misfire();
		return;
	}

	//If we dont have battery larger than the emp depletion rate then we return. 
	if (BatteryComponent->CurrentPower < BatteryDepletion) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("No battery"));
		return;
	}

	//If gun is in cooldown after shoot lamp
	const float CurrentTime = GetWorld()->TimeSeconds;
	if (CurrentTime - LastFireTime < FireCoolDown) {
		if (GEngine)
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Still cooling down"));
		return;
	}
	LastFireTime = CurrentTime;

	//Remove battery power corresponding to our depletion rate.
	BatteryComponent->CurrentPower -= BatteryDepletion;


	//Play Emp sound the the emp blueprint
	PlayEmpSound();

	//Turn of the light
	ALightActor* LightActorEffected = Cast<ALightActor>(TargetLight->GetOwner());
	LightActorEffected->IsTurnedOfEmp = true;
}


void AEmpGun::BlueprintLineTraceFromPlayer()
{
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	//CollisionQueryParams.AddIgnoredActor(GP3Character);


	//If we have a player then get the get the players camera location and get the forwards direction from it.
	if (!GP3Character) return;
	FVector StartLocation = GP3Character->FirstPersonCameraComponent->GetComponentLocation();
	FVector EndForward = GP3Character->FirstPersonCameraComponent->GetForwardVector() * Distance;
	FVector EndLocation = EndForward + StartLocation;

	//Do a line trace from the player camera location forwards using the EMP collision channel. 
	//GetWorld()->LineTraceSingleByChannel(HitResultEmp, StartLocation, EndLocation, ECC_EMP, CollisionQueryParams);
	GetWorld()->LineTraceSingleByChannel(HitResultEmp, StartLocation, EndLocation, ECC_Visibility, CollisionQueryParams);



	if (HitResultEmp.bBlockingHit)
	{
		//If we hit something we check if its a light actor.
		//if (!HitResultEmp.GetActor()->IsA<ALightActor>()) return;

		//If we hit something we check if its a light actor. If not then remove the current light target becuase now it is not in line of sight, then return;
		if (!HitResultEmp.GetActor()->IsA<ALightActor>()) 
		{
			TargetLight = nullptr;
			return;
		}


		HitResultEmp.GetActor()->GetComponents(Lights, true);

		//If the light component on the light actor is not visible or the light intensity is 0 then we dont continue.
		if ((!Lights[0]->IsVisible()) || (Lights[0]->Intensity == 0)) return;

		//If the light is visible and have a light intensity that is not 0 then we assign it as our target light. 
		TargetLight = Lights[0];
	}
	else
	{
		//Remove the Target light. 
		TargetLight = nullptr;
	}
}

void AEmpGun::Misfire()
{
	

	//Delay on mis fire
	const float CurrentTime = GetWorld()->TimeSeconds;
	if (CurrentTime - LastFireTime < MissCoolDown) return;
	LastFireTime = CurrentTime;

	//Mis fire blueprint event triggered
	EmpMisfire();
}
