// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/TriggerVolume.h"
#include "open_door.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDING_ESCAPE_API Uopen_door : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	Uopen_door();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void open_door();

	void close_door();

	void move_door(float &yaw_amount, bool log = false);

private:
	UPROPERTY(VisibleAnywhere)
	float open_angle = 90.0f;

	UPROPERTY(EditAnywhere)
	ATriggerVolume* pressure_plate;

	UPROPERTY(EditAnywhere)
	AActor* actor_that_opens; // remember pawn inherits from actor

	UPROPERTY(EditAnywhere)
	float door_close_delay;

	float last_door_open_time;
	AActor *owner;
	bool is_open;

};
