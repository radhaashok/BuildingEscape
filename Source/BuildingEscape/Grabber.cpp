// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include <Engine/World.h>
#include <Public/DrawDebugHelpers.h>

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));
	
	FindPhysicsHandleComponent();

	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	/// Look for attached Input Component (only appears at run time)
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("Input component found"));
		/// Bind the input axis
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing input component"), *GetOwner()->GetName());
	}
}

void UGrabber::FindPhysicsHandleComponent()
{
	/// Look for attached physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle)
	{
		/// Physics handle is found
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s missing physics handle component"), *GetOwner()->GetName());
	}
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed"));
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released"));

	// Try and reach any actors with physics body collision channel set

	// If we hit something then attach a physics handle
	// TODO attach physics handle

}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// LINE TRACE if the physics handle is attached
	// move the object that we're holding

	GetFirstPhysicsBodyInReach();
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	/// Get player view point this tick
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);
	//UE_LOG(LogTemp, Warning, TEXT("Player Location: %s"), *PlayerViewPointLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Player Rotation: %s"), *PlayerViewPointRotation.ToString());

	/// Ray-cast out to reach distance
	FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;

	/// Setup query parameters
	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	/// Line-trace (AKA ray-cast) out to reach distance
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/// See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit) {
		UE_LOG(LogTemp, Warning, TEXT("Line trace hit: %s"), *ActorHit->GetName())
	}

	return Hit;
}

