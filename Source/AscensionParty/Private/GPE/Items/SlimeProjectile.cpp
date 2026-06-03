// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/SlimeProjectile.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include <Player/PlayerCharacter.h>
#include "Slime.h"
// Sets default values
ASlimeProjectile::ASlimeProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    // SphereComponent devient le root
    sphereHit = CreateDefaultSubobject<USphereComponent>("Sphere");
    RootComponent = sphereHit; // ← sphere EST le root

    body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
    movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");

    body->SetupAttachment(RootComponent);
    body->SetRelativeLocation(FVector::ZeroVector);

    // Movement update directement la sphere
    movement->SetUpdatedComponent(sphereHit);
    movement->bRotationFollowsVelocity = false;
	sphereHit->IgnoreActorWhenMoving(this, true);
}

// Called when the game starts or when spawned
void ASlimeProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}
void ASlimeProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse,
	const FHitResult& Hit)
{
	if (OtherActor == this)return;
	
	if (!shooterRef|| OtherActor == shooterRef||Cast<ASlimeProjectile>(OtherActor)|| Cast<ASlime>(OtherActor))
	{
		return;
	}
	APlayerCharacter* _player = Cast<APlayerCharacter>(OtherActor);
	if (_player && _player->GetIsDead())
		return;
	UKismetSystemLibrary::PrintString(this, OtherActor->GetActorNameOrLabel());
	Spawn(OtherActor);
	
	this->Destroy();
	
	
}

void ASlimeProjectile::OnHitTest(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (_otherActor == this)return;

	if (!shooterRef || _otherActor == shooterRef || Cast<ASlimeProjectile>(_otherActor) || Cast<ASlime>(_otherActor))
	{
		return;
	}
	APlayerCharacter* _player = Cast<APlayerCharacter>(_otherActor);
	if (_player && _player->GetIsDead())
		return;
	UKismetSystemLibrary::PrintString(this, _otherActor->GetActorNameOrLabel());
	Spawn(_otherActor);

	this->Destroy();
}

// Called every frame
void ASlimeProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASlimeProjectile::Spawn(AActor* _hitActor)
{
	sphereHit->SetGenerateOverlapEvents(false);
	if (!zone)
	{
		UKismetSystemLibrary::PrintString(this, " pas de zone");
		return;

	}
	ASlime* _slime = GetWorld()->SpawnActor<ASlime>(zone, GetActorLocation(), FRotator(0,180,0));
	_slime->SetHitActor(_hitActor);
	_slime->SetShooter(shooterRef);
	sphereHit->IgnoreActorWhenMoving(_slime, true);
	_slime->GetBox()->IgnoreActorWhenMoving(this,true);
	zone = nullptr;
	//this->Destroy();
}

void ASlimeProjectile::SetShooterRef(APlayerCharacter* _player)
{
	shooterRef = _player;
	sphereHit->IgnoreActorWhenMoving(_player, true);
	sphereHit->IgnoreActorWhenMoving(this, true);
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		sphereHit->OnComponentHit.AddDynamic(this, &ASlimeProjectile::OnHit);
		//sphereHit->OnComponentBeginOverlap.AddDynamic(this, &ASlimeProjectile::OnHitTest);
	}
}

