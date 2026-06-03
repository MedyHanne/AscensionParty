// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Obstacles/ObstacleBumper.h"
#include <Player/PlayerCharacter.h>
#include<Player/Component/PlayerMovementComponent.h>
#include <Kismet/KismetSystemLibrary.h>
#include "ObstacleIceBlock.h"

// Sets default values
AObstacleBumper::AObstacleBumper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

}

// Called when the game starts or when spawned
void AObstacleBumper::BeginPlay()
{
	Super::BeginPlay();
	box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
	box->OnComponentBeginOverlap.AddDynamic(this, &AObstacleBumper::OnWalkIn);

	}
}

// Called every frame
void AObstacleBumper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AObstacleBumper::OnWalkIn(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(_otherActor);
	

	if (_player)
	{
		
		_player->LaunchCharacter(FVector(0, 0, launchZ), true, true);

	}
}



