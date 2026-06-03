// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Projectiles/BaseProjectile.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/SphereComponent.h>
#include <GameFramework/ProjectileMovementComponent.h>
#include "../PlayerCharacter.h"
#include "../Component/PlayerMovementComponent.h"




// Sets default values
ABaseProjectile::ABaseProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	sphereHit = CreateDefaultSubobject<USphereComponent>("Sphere");
	hand = CreateDefaultSubobject<USkeletalMeshComponent>("Hand");
	movement = CreateDefaultSubobject<UProjectileMovementComponent>("Movement");

	RootComponent = sphereHit;
	hand->SetupAttachment(RootComponent);
	AddOwnedComponent(movement);
	/*movement->SetUpdatedComponent(sphereHit);
	movement->SetIsReplicated(true);*/
}

// Called when the game starts or when spawned
void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	/*UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(movement->InitialSpeed));
	UKismetSystemLibrary::PrintString(this, movement->UpdatedComponent->GetName());
	UKismetSystemLibrary::PrintString(this, GetActorForwardVector().ToString());*/
	
	movement->Velocity = GetActorForwardVector() * movement->InitialSpeed;
	if (shooterRef)
	{
		int _idController = shooterRef->GetIdController();
		Multicast_ReplicateInit(shooterRef, this, _idController);
	}
}

// Called every frame
void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UKismetSystemLibrary::DrawDebugLine(this, GetActorLocation(), GetActorLocation() + GetActorForwardVector() * 50,FLinearColor::Blue);

}

void ABaseProjectile::OnHit( UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult)
{
	if (_otherActor == shooterRef)return;
	APlayerCharacter* _player = Cast<APlayerCharacter>(_otherActor);

		
		if (!_player||_player->GetHitProjectile())
		{
			OnlineDestroy();
			return;
		}
		if (_player->GetIsDead())return;
		if (!HasAuthority())
		{
			Server_ReplicateGrab(this, _player);
		}
		else
		{
			Multicast_ReplicateGrab(this, _player);
		}
	
	
}

void ABaseProjectile::Server_ReplicateGrab_Implementation(ABaseProjectile* _projectile, APlayerCharacter* _player)
{
	Multicast_ReplicateGrab(_projectile, _player);
}

void ABaseProjectile::Multicast_ReplicateGrab_Implementation(ABaseProjectile* _projectile, APlayerCharacter* _player)
{
	_projectile->hitPlayerBefore = true;
	_player->SetHitProjectile(true);
	FVector _dir = _projectile->shooterRef->GetActorLocation() - _projectile->movement->UpdatedPrimitive->GetComponentLocation();
	_dir.X = 0;
	_dir.Normalize();
	_projectile->direction = _dir;
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, [_projectile, _player]() {

		UPlayerMovementComponent* _movement = _player->GetMovement();
		_movement->GroundFriction = 0;
		_movement->FallingLateralFriction = 0;
		_movement->BrakingFriction = 0;

		_player->LaunchCharacter(_projectile->direction * _projectile->knockback, true, true);
		_player->SetIsKnockback(true);

		}, 0.5f, false);

	_projectile->sphereHit->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	if (_projectile->animToPlay)
		_projectile->hand->PlayAnimation(animToPlay, false);

	UKismetSystemLibrary::PrintString(_projectile, "Stop");
	_projectile->movement->StopMovementImmediately();

	FAttachmentTransformRules _attach = FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, true);


	_projectile->AttachToActor(_player, _attach);

	FTimerHandle _endTimer;
	GetWorld()->GetTimerManager().SetTimer(_endTimer, [_projectile, _player]() {

		_projectile->hitPlayerBefore = false;
		_player->SetIsKnockback(false);
		_player->SetHitProjectile(false);
		UPlayerMovementComponent* _movement = _player->GetMovement();
		_movement->GroundFriction = 8;
		_movement->FallingLateralFriction = 5;
		_movement->BrakingFriction = 2;
		_projectile->OnlineDestroy();


		}, 0.6f, false);
}

void ABaseProjectile::OnlineDestroy()
{

	if (!HasAuthority())
	{
		Server_ReplicateDestroy(this);

	}
	else
	{
		Destroy();
	}
}

void ABaseProjectile::Server_ReplicateDestroy_Implementation(ABaseProjectile* _projectile)
{
	_projectile->Destroy();
}

void ABaseProjectile::Multicast_ReplicateInit_Implementation(APlayerCharacter* _shooter,ABaseProjectile* _projectile, int _idController)
{
	shooterRef = _shooter;
	if (materialProjectile.Num() < _idController)return;
	UMaterialInstance* _mat = materialProjectile[_idController];
	if (_mat)
	{
		hand->SetMaterial(0, _mat);
	}
	sphereHit->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnHit);
}

