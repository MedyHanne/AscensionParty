// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Lever.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ALever::ALever()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	pivot = CreateDefaultSubobject<USceneComponent>("Pivot");
	lever = CreateDefaultSubobject<UStaticMeshComponent>("Lever");

	pivot->SetupAttachment(RootComponent);
	lever->SetupAttachment(pivot);

}

// Called when the game starts or when spawned
void ALever::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{

		box->OnComponentBeginOverlap.AddDynamic(this, &ALever::OnWalkIn);
		UKismetSystemLibrary::PrintString(this, "Init Levier");


	}
}

// Called every frame
void ALever::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ALever::OnWalkIn(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{

	box->OnComponentBeginOverlap.RemoveDynamic(this, &ALever::OnWalkIn);
	for (TObjectPtr<AActor> _actor : activables)
	{
		if (_actor && _actor->Implements<ULeverActivableInterface>())
			ILeverActivableInterface::Execute_Activate(_actor);
	}
	Multicast_ReplicateRotate(this, FRotator(0, 0, -40));
	UKismetSystemLibrary::PrintString(this, "Levier");


}

void ALever::Multicast_ReplicateRotate_Implementation(ALever* _lever, FRotator _rotation)
{
	_lever->pivot->SetRelativeRotation(_rotation);
}

