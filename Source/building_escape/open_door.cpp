// Fill out your copyright notice in the Description page of Project Settings.

#include "open_door.h"
#include <math.h>
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
Uopen_door::Uopen_door()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void Uopen_door::BeginPlay()
{
	Super::BeginPlay();



	
}


// Called every frame
void Uopen_door::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	AActor *owner = GetOwner();

	static int frame = 0;
	static float yaw_change = 0;
	const int FRAMES_TILL_CHANGE = 1;
	static float yaw_amount = 0.25;

	frame += 1;
	if (frame == FRAMES_TILL_CHANGE) {
		FRotator curr_rot = owner->GetActorRotation();
		FRotator new_rot = FRotator(curr_rot.Pitch, curr_rot.Yaw + yaw_amount, curr_rot.Roll);

		if (yaw_change >= 90.0) {
			FString object_name = owner->GetName();
			UE_LOG(LogTemp, Warning, TEXT("performed rotation on %s, yaw_amount is %f, yaw_change is %f"), *object_name, yaw_amount, yaw_change);

			yaw_amount *= -1;
			yaw_change = 0;
		}

		owner->SetActorRotation(new_rot, static_cast<ETeleportType>(NULL));
		yaw_change += fabs(yaw_amount);

		frame = 0;

	}


}

