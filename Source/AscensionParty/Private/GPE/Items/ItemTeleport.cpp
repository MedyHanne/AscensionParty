// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/ItemTeleport.h"
#include <Player/PlayerCharacter.h>
#include <Kismet/GameplayStatics.h>


// Sets default values
AItemTeleport::AItemTeleport()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	hitSphere = CreateDefaultSubobject<UStaticMeshComponent>("HitSphere");
	splineChain = CreateDefaultSubobject<USplineComponent>("SplineChain");
	splineMesh = CreateDefaultSubobject<USplineMeshComponent>("SplineMesh");
	sphereOverlap = CreateDefaultSubobject<USphereComponent>("SphereOverlap");

	hitSphere->SetupAttachment(RootComponent);
	sphereOverlap->SetupAttachment(hitSphere);
	splineChain->SetupAttachment(RootComponent);
	splineMesh->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AItemTeleport::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
	hitSphere->OnComponentBeginOverlap.AddDynamic(this, &AItemTeleport::OnHitSphereBeginOverlap);
	hitSphere->OnComponentEndOverlap.AddDynamic(this, &AItemTeleport::OnHitSphereEndOverlap);
	sphereOverlap->OnComponentEndOverlap.AddDynamic(this, &AItemTeleport::OnSphereOverlapEndOverlap);

	}
	
}

void AItemTeleport::Use()
{
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		GetWorld()->GetTimerManager().SetTimer(updatePlayerTimer, this, &AItemTeleport::Update, updateRate, true);
		GetWorld()->GetTimerManager().SetTimer(endTimer, this, &AItemTeleport::Finish, teleportDelay, false);
		Multicast_ReplicateUseTeleport(this);
	}
	else
	{
		Server_ReplicateUseTeleport(this);
	}
}

// Called every frame
void AItemTeleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemTeleport::OnHitSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(OtherActor);
	if (!_player||_player==refUser||_player->GetIsDead())return;
	inRangePlayers.Add(_player);
}

void AItemTeleport::OnHitSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(OtherActor);
	if (!_player || _player == refUser)return;
	inRangePlayers.Remove(_player);
}

void AItemTeleport::OnSphereOverlapEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}
void AItemTeleport::Multicast_ReplicateUseTeleport_Implementation(AItemTeleport* _item)
{
	_item->AttachToComponent(refUser->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	_item->SetActorRelativeLocation(FVector(0, 0, 0));
}
void AItemTeleport::Server_ReplicateUseTeleport_Implementation(AItemTeleport* _item)
{
	GetWorld()->GetTimerManager().SetTimer(updatePlayerTimer, this, &AItemTeleport::Update, updateRate, true);
	GetWorld()->GetTimerManager().SetTimer(endTimer, this, &AItemTeleport::Finish, teleportDelay, false);

	Multicast_ReplicateUseTeleport(_item);
}
void AItemTeleport::Multicast_ReplicateUpdateSphere_Implementation(AItemTeleport* _item, float _time)
{
	progress += updateRate;
	float _scaleTarget = radiusmax / 50;
	FVector _newScale = FVector((_scaleTarget *progress/teleportDelay));
	
	_item->hitSphere->SetRelativeScale3D(_newScale);
	if (_time > 0.2)
	{
		if (_time > 0.4f)
		{
			_item->hitSphere->SetMaterial(0, _item->redMat);
		}
		else
		{
			_item->hitSphere->SetMaterial(0, _item->orangeMat);

		}

	}
}
void AItemTeleport::Multicast_ReplicateUpdateSpline_Implementation(AItemTeleport* _item, APlayerCharacter* _player,  APlayerCharacter* _target)
{
	if (!_target)
	{
		_item->splineMesh->SetVisibility(false);
		return;
	}
	_item->splineMesh->SetVisibility(true);
	_item->splineChain->ClearSplinePoints(true);
	_item->splineChain->AddSplinePoint(_target->GetActorLocation(), ESplineCoordinateSpace::World, true);
	_item->splineChain->AddSplinePoint(_player->GetActorLocation(), ESplineCoordinateSpace::World, true);


	FVector _startPos = _item->splineChain->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector _startTangent = _item->splineChain->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector _endPos = _item->splineChain->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	FVector _endTangent = _item->splineChain->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::Local);

	_item->splineMesh->SetStartAndEnd(_startPos, _startTangent, _endPos, _endTangent, true);
}
void AItemTeleport::UpdatePlayers()
{
	float _distance = 0;
	TArray<AActor*> _actors;
	for (APlayerCharacter* _player : inRangePlayers)
	{
		_actors.Add(_player);
	}
	
	nearestPlayer = Cast<APlayerCharacter>(UGameplayStatics::FindNearestActor(refUser->GetActorLocation(), _actors, _distance));
}
void AItemTeleport::Finish()
{
	UpdatePlayers();
	GetWorld()->GetTimerManager().ClearTimer(updatePlayerTimer);
	if (nearestPlayer)
	{
		FVector _start = refUser->GetActorLocation();
		FVector _end = nearestPlayer->GetActorLocation();

		refUser->SetActorLocation(_end);
		nearestPlayer->SetActorLocation(_start);
	}
	Super::Use();

}
void AItemTeleport::UpdateSpline()
{
	/*splineChain->ClearSplinePoints(true);*/
	//if (!nearestPlayer)return;

	/*splineChain->AddSplinePoint(refUser->GetActorLocation(), ESplineCoordinateSpace::World, true);
	splineChain->AddSplinePoint(nearestPlayer->GetActorLocation(), ESplineCoordinateSpace::World, true);

	
	FVector _startPos = splineChain->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector _startTangent = splineChain->GetTangentAtSplinePoint(0, ESplineCoordinateSpace::Local);
	FVector _endPos = splineChain->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	FVector _endTangent = splineChain->GetTangentAtSplinePoint(1, ESplineCoordinateSpace::Local);*/
	
	//splineMesh->SetStartAndEnd(_startPos, _startTangent, _endPos, _endTangent, true);
	Multicast_ReplicateUpdateSpline(this, refUser, nearestPlayer);
}
void AItemTeleport::UpdateSphere()
{
	float _timer = GetWorld()->GetTimerManager().GetTimerElapsed(endTimer);
	Multicast_ReplicateUpdateSphere(this, _timer);
}
void AItemTeleport::Update()
{
	UpdatePlayers();
	UpdateSpline();
	UpdateSphere();
}


