// Fill out your copyright notice in the Description page of Project Settings.

#include "open_door.h"
#include <math.h>
#include "GameFramework/Actor.h"
#include "Engine/World.h"

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
	actor_that_opens = GetWorld()->GetFirstPlayerController()->GetPawn();
}

// Called every frame
void Uopen_door::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (!is_open && pressure_plate->IsOverlappingActor(actor_that_opens)) {
		open_door();
	}

	if (is_open && !pressure_plate->IsOverlappingActor(actor_that_opens)) {
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

