// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
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

FVector UGrabber::get_reach_line_start()
{
	// get player viewpoint (location, rotation)
	FVector player_viewpoint_location;
	FRotator player_viewpoint_rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT player_viewpoint_location,
		OUT player_viewpoint_rotation
	);

	return player_viewpoint_location;
}

FVector UGrabber::get_reach_line_end()
{
	// get player viewpoint (location, rotation)
	FVector player_viewpoint_location;
	FRotator player_viewpoint_rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT player_viewpoint_location,
		OUT player_viewpoint_rotation
	);

	return player_viewpoint_location + (player_viewpoint_rotation.Vector() * Reach);
}

const FHitResult UGrabber::get_first_object_in_reach(ECollisionChannel collision_channel)
{
	// perform line trace
	FCollisionQueryParams trace_params = FCollisionQueryParams(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;
	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		get_reach_line_start(),
		get_reach_line_end(),
		FCollisionObjectQueryParams(collision_channel),
		trace_params
	);

	return Hit;
}

void UGrabber::find_physics_handle_component()
{
	physics_handle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (physics_handle == nullptr) {
		UE_LOG(LogTemp, Error, TEXT("%s missing UPhysicsHandleComponent"), *(GetOwner()->GetName()))
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// if the physics handle is attached
		// move object
	if (physics_handle->GrabbedComponent)
	{
		physics_handle->SetTargetLocation(get_reach_line_end());
	}

}

void UGrabber::grab()
{
	//line trace and try and reach any actors with physics body collision channel set
	auto hit_result = get_first_object_in_reach(ECollisionChannel::ECC_PhysicsBody);
	auto component_to_grab = hit_result.GetComponent();

	// TODO if we hit something attach physics handle to it
	if (component_to_grab) {
		physics_handle->GrabComponent(
			component_to_grab,
			NAME_None,
			component_to_grab->GetOwner()->GetActorLocation(),
			true
		);
	}
}

void UGrabber::release()
{
	physics_handle->ReleaseComponent();
}
