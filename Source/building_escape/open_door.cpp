// Fill out your copyright notice in the Description page of Project Settings.

#include "open_door.h"
#include <math.h>
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#define OUT

// Sets default values for this component's properties
Uopen_door::Uopen_door()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	is_open = false;
}


// Called when the game starts
void Uopen_door::BeginPlay()
{
	Super::BeginPlay();
	owner = GetOwner();
}

float Uopen_door::get_total_mass_of_actors_on_plate()
{
	static float last_total_mass = 0.f;
	float total_mass = 0.f;
	TArray<AActor*> overlapping_actors;

	pressure_plate->GetOverlappingActors(
		OUT overlapping_actors
	);

	for (const auto &actor : overlapping_actors) 
	{
		total_mass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("actor %s is on trigger."), *actor->GetName())
	}

	if (last_total_mass != total_mass)
	{
		last_total_mass = total_mass;
		UE_LOG(LogTemp, Warning, TEXT("total mass is %f"), last_total_mass);
	}
	
	return total_mass;
}

// Called every frame
void Uopen_door::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	bool sufficient_mass_on_plate = get_total_mass_of_actors_on_plate() >= trigger_mass;
	if (!is_open && sufficient_mass_on_plate)
	{
		open_door();
	}

	if (is_open && !sufficient_mass_on_plate)
	{
		close_door();
	}
}

void Uopen_door::open_door()
{
	float yaw_amount = 90.0;
	move_door(yaw_amount);
	is_open = true;
}
void Uopen_door::close_door()
{
	float yaw_amount = -90.0;
	move_door(yaw_amount);
	is_open = false;
}

void Uopen_door::move_door(float &yaw_amount, bool log)
{
	AActor *owner = GetOwner();
	FRotator curr_rot = owner->GetActorRotation();
	FRotator new_rot = FRotator(curr_rot.Pitch, curr_rot.Yaw + yaw_amount, curr_rot.Roll);

	FString object_name;
	if (log) {
		object_name = owner->GetName();
		UE_LOG(LogTemp, Warning, TEXT("performed rotation on %s, rotated %f amount"), *object_name, yaw_amount)
	}


	owner->SetActorRotation(new_rot, ETeleportType::None);

}

