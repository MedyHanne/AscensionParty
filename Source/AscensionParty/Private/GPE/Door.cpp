// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Door.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	center = CreateDefaultSubobject<UStaticMeshComponent>("Center");
	door = CreateDefaultSubobject<UStaticMeshComponent>("Door");

	center->SetupAttachment(RootComponent);
	door->SetupAttachment(center);
	bReplicates = true;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoor::Activate_Implementation()
{
	GetWorld()->GetTimerManager().SetTimer(rotateTimer, this, &ADoor::Rotate, 0.01f, true);
}

void ADoor::Rotate()
{
	UKismetSystemLibrary::PrintString(this, "Rotate");

	FRotator _currentRot = center->GetRelativeRotation();
	FRotator _newRotation = FMath::RInterpConstantTo(_currentRot, finalRotation, 0.01, 90.f);

	Multicast_ReplicateRotate(this,_newRotation);

	if (_newRotation.Equals(finalRotation, 1.0f))
	{
		center->SetRelativeRotation(finalRotation);
		Multicast_ReplicateRotate(this, finalRotation);

		GetWorld()->GetTimerManager().ClearTimer(rotateTimer);
		UKismetSystemLibrary::PrintString(this, "Finish rotate");

	}

}

void ADoor::Multicast_ReplicateRotate_Implementation(ADoor* _door, FRotator _rotation)
{
	_door->center->SetRelativeRotation(_rotation);
}

