// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	find_physics_handle_component();
	setup_input_component();
}

void UGrabber::setup_input_component()
{
	input_comp = GetOwner()->FindComponentByClass<UInputComponent>();
	if (input_comp) {
		input_comp->BindAction("Grab", IE_Pressed, this, &UGrabber::grab);
		input_comp->BindAction("Grab", IE_Released, this, &UGrabber::release);
	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing UInputComponent"), *(GetOwner()->GetName()))
	}
}

const FHitResult UGrabber::get_first_physics_body_in_reach()
{
	FVector PlayerViewPointLocation;
	FRotator PlayerViewPointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PlayerViewPointLocation,
		OUT PlayerViewPointRotation
	);

	//UE_LOG(LogTemp, Warning, TEXT("location %s. rotation %s"), *PlayerViewPointLocation.ToString(), *PlayerViewPointRotation.ToString())
	FVector LineTraceEnd = PlayerViewPointLocation + (PlayerViewPointRotation.Vector() * Reach);
	DrawDebugLine(
		GetWorld(),
		PlayerViewPointLocation,
		LineTraceEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0,
		10.f
	);

	FCollisionQueryParams trace_params = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PlayerViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		trace_params
	);
	AActor *hit_actor = Hit.GetActor();
	if (hit_actor) {
		UE_LOG(LogTemp, Warning, TEXT("hit actor: %s"), *(hit_actor->GetName()))
	}

	return Hit;
}

void UGrabber::find_physics_handle_component()
{
	physics_handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physics_handle) {

	}
	else {
		UE_LOG(LogTemp, Error, TEXT("%s missing UPhysicsHandleComponent"), *(GetOwner()->GetName()))
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
		// move object


}

void UGrabber::grab()
{
	UE_LOG(LogTemp, Warning, TEXT("grabbin ya!"))

	//line trace and try and reach any actors with physics body collision channel set
	get_first_physics_body_in_reach();
	// TODO if we hit something attach physics handle to it
}

void UGrabber::release()
{
	UE_LOG(LogTemp, Warning, TEXT("released ya!"))

	// TODO release handleS
}


