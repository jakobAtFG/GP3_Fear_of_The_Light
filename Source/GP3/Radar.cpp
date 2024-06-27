// Fill out your copyright notice in the Description page of Project Settings.

#include "Radar.h"
#include "Components/CanvasPanel.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/WidgetLayoutLibrary.h"

// Sets default values for this component's properties
URadar::URadar()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URadar::BeginPlay()
{
	Super::BeginPlay();

	if (RadarWidgetClass != nullptr)
	{
		RadarWidget = CreateWidget(GetWorld(), RadarWidgetClass, FName(TEXT("RadarWidget")));
		RadarWidget->AddToViewport();

		UCanvasPanel* Canvas = Cast<UCanvasPanel>(RadarWidget->GetRootWidget());
		RadarImage = Cast<UImage>(Canvas->GetChildAt(0));

		// log radar image name
		UE_LOG(LogTemp, Warning, TEXT("Radar Image Name: %s"), *RadarImage->GetName());

		PlaceBlip(GetOwner()); // Place the player's blip on the radar
	}
	
}


// Called every frame
void URadar::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void URadar::PlaceBlip(AActor* Actor)
{
	// Get the actor's location
	FVector ActorLocation = Actor->GetActorLocation();

	// Get the player's location
	FVector PlayerLocation = GetOwner()->GetActorLocation();

	// Get the player's rotation
	FRotator PlayerRotation = GetOwner()->GetActorRotation();

	// Get the direction from the player to the actor
	FVector Direction = ActorLocation - PlayerLocation;

	// Get the distance from the player to the actor
	float Distance = Direction.Size();

	Direction.Normalize();

	// Get the angle between the player's forward vector and the direction to the actor
	float PlayerForwardToActorAngle = FMath::RadiansToDegrees(
		FMath::Acos(
			FVector::DotProduct(
				PlayerRotation.Vector(),
				Direction
			)
		)
	);

	// If the actor is within the radar's range and angle
	if (true)
	{
		// Create blip widget
		UUserWidget* BlipWidget = CreateWidget(RadarWidget, BlipWidgetClass, FName(TEXT("BlipWidget")));

		UCanvasPanelSlot* BlipSlot = Cast<UCanvasPanelSlot>(BlipWidget);
		Cast<UCanvasPanel>(RadarWidget)->AddChild(BlipWidget);

		// Get dimensions and position of the radar image
		FVector2D RadarImageSize = Cast<UCanvasPanelSlot>(RadarImage)->GetSize();
		FVector2D RadarImagePosition = Cast<UCanvasPanelSlot>(RadarImage)->GetPosition();
		FAnchors RadarImageAnchors = Cast<UCanvasPanelSlot>(RadarImage)->GetAnchors();

		// Calculate the position of the blip on the radar
		FVector2D CalculatedPosition = FVector2D(RadarImagePosition) + RadarImageSize.X / 2 + RadarImageSize.Y;

		// Set the position of the blip on the radar
		BlipSlot->SetAnchors(RadarImageAnchors);
		BlipSlot->SetPosition(CalculatedPosition);

		// log position
		UE_LOG(LogTemp, Warning, TEXT("Blip Position: %f, %f"), CalculatedPosition.X, CalculatedPosition.Y);

	}
}