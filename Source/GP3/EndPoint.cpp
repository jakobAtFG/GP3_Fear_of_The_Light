// Fill out your copyright notice in the Description page of Project Settings.


#include "EndPoint.h"

#include "GameManager.h"
#include "GameFramework/Character.h"

// Sets default values
AEndPoint::AEndPoint()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("Collision Box");

	RootComponent = CollisionBox;

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &AEndPoint::HandleOverlap);
	CollisionBox->ShapeColor = FColor::Green;
	CollisionBox->SetLineThickness(2.f);
}

// Called when the game starts or when spawned
void AEndPoint::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEndPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndPoint::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                              const FHitResult& SweepResult)
{
	if (OtherActor->IsA(ACharacter::StaticClass()))
	{
		if(GetWorld()->GetGameInstance()->IsA(UGameManager::StaticClass()))
		{
			UGameManager* GameManager = Cast<UGameManager>(GetWorld()->GetGameInstance());
			GameManager->OnWinGame();
		}
	}
}
