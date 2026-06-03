// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/Slime.h"
#include <Player/PlayerCharacter.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
ASlime::ASlime()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	slimeBody = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	cylinder = CreateDefaultSubobject<UStaticMeshComponent>("Cylinder");
	box = CreateDefaultSubobject<UBoxComponent>("Box");

	slimeBody->SetupAttachment(RootComponent);
	cylinder->SetupAttachment(slimeBody);
	box->SetupAttachment(slimeBody);
}

// Called when the game starts or when spawned
void ASlime::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASlime::SetShooter(APlayerCharacter* _player)
{
	Multicast_ReplicateSetShooter(this, _player);

}

void ASlime::SetHitActor(AActor* _actor)
{
	Multicast_ReplicateSetHitActor(this, _actor);

	

}

// Called every frame
void ASlime::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlime::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(OtherActor);
	if (!_player || _player == shooterRef)return;

	Multicast_ReplicateOverlap(_player, 0.5);
}

void ASlime::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* _player = Cast<APlayerCharacter>(OtherActor);
	if (!_player || _player == shooterRef)return;

	Multicast_ReplicateOverlap(_player, 1);

}

void ASlime::Multicast_ReplicateOverlap_Implementation( APlayerCharacter* _player, float _scustomTimeDilation)
{
	UKismetSystemLibrary::PrintString(_player,FString::FromInt(_scustomTimeDilation));

	_player->CustomTimeDilation = _scustomTimeDilation;
}

void ASlime::Multicast_ReplicateSetHitActor_Implementation(ASlime* _slime, AActor* _actor)
{
	_slime->hitActor = _actor;

	FVector _newScale = FVector(_actor->GetActorRelativeScale3D().X, 6, 1);
	_slime->slimeBody->SetRelativeScale3D(_newScale);
	_slime->cylinder->SetWorldScale3D(_newScale + 0.5f);
}

void ASlime::Multicast_ReplicateSetShooter_Implementation(ASlime* _slime, APlayerCharacter* _player)
{
	_slime->shooterRef = _player;

	UMaterialInstance* _mat = _slime->materialOverlay[_slime->shooterRef->GetIdController()];
	if (_mat)
	{
		_slime->cylinder->SetMaterial(0, _mat);
	}
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		box->OnComponentBeginOverlap.AddDynamic(this, &ASlime::OnBoxBeginOverlap);
		box->OnComponentEndOverlap.AddDynamic(this, &ASlime::OnBoxEndOverlap);
	}
	TArray<AActor*> _overlapping;

	box->GetOverlappingActors(_overlapping);
	for (AActor* _actor : _overlapping)
	{
		APlayerCharacter* _player = Cast<APlayerCharacter>(_actor);
		if (!_player || _player == shooterRef)continue;
		Multicast_ReplicateOverlap(_player, 0.5);
	}
}

