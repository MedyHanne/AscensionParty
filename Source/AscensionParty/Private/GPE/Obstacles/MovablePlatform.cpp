// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Obstacles/MovablePlatform.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
AMovablePlatform::AMovablePlatform()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	platform = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	movement = CreateDefaultSubobject<UInterpToMovementComponent>("Movement");

	platform->SetupAttachment(RootComponent);
	movement->SetUpdatedComponent(RootComponent);
	bReplicates = true;
	//SetReplicateMovement(true);

}

// Called when the game starts or when spawned
void AMovablePlatform::BeginPlay()
{
	Super::BeginPlay();
	/*movement->FinaliseControlPoints();
	movement->RestartMovement();
	Server_Synchronize(this);*/
	movement->StopMovementImmediately();
}

void AMovablePlatform::OnConstruction(const FTransform& Transform)
{
    Super::OnConstruction(Transform);
    movement->ControlPoints = wayPoints;
   

}
void AMovablePlatform::Activate()
{
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		UKismetSystemLibrary::PrintString(this, "Activate Server");

		Multicast_ReplicateStart(this);
	}
	else
	{
	UKismetSystemLibrary::PrintString(this, "Activate Client");
	Server_ReplicateStart(this);

	}


}

void AMovablePlatform::Server_ReplicateStart_Implementation(AMovablePlatform* _platform)
{
	UKismetSystemLibrary::PrintString(_platform, "ServerStart");

	Multicast_ReplicateStart(_platform);

}

// Called every frame
void AMovablePlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (canActivate)
	{
		Activate();
		//canActivate = false;
	}
}

void AMovablePlatform::StartMovement()
{
	movement->RestartMovement();
}

void AMovablePlatform::Multicast_ReplicateStart_Implementation(AMovablePlatform* _platform)
{
	_platform->movement->RestartMovement();
	UKismetSystemLibrary::PrintString(_platform, "Restart");
}

void AMovablePlatform::Multicast_Synchronize_Implementation(AMovablePlatform* _platform, FVector _position)
{
	/*_platform->SetActorLocation(_position);
	_platform->movement->SetCurrentT*/
}

void AMovablePlatform::Server_Synchronize_Implementation(AMovablePlatform* _platform)
{
	Multicast_Synchronize(_platform, _platform->GetActorLocation());
}

