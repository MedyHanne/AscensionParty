// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Teleport.h"
#include <Player/PlayerCharacter.h>
#include <Player/Component/ProjectileComponent.h>
#include <Player/Projectiles/BaseProjectile.h>

// Sets default values
ATeleport::ATeleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent=CreateDefaultSubobject<USceneComponent>("Root");
	teleport=CreateDefaultSubobject<UStaticMeshComponent>("Teleport");
	spawn =CreateDefaultSubobject<USceneComponent>("Spawn");

	teleport->SetupAttachment(RootComponent);
	spawn->SetupAttachment(teleport);

}

// Called when the game starts or when spawned
void ATeleport::BeginPlay()
{
	Super::BeginPlay();
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		UKismetSystemLibrary::PrintString(this, "BindTeleport");

		teleport->OnComponentBeginOverlap.AddDynamic(this, &ATeleport::OnBoxBeginOverlap);
	}
}

// Called every frame
void ATeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void ATeleport::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (Cast<APlayerCharacter>(OtherActor) || Cast<ABaseProjectile>(OtherActor))
	{
	
		FVector _newLoc =teleportRef->spawn->GetComponentLocation();
		FRotator _newRot = teleportRef->GetRootComponent()->GetComponentRotation();
		
		OtherActor->TeleportTo(_newLoc, _newRot);

	}

}

