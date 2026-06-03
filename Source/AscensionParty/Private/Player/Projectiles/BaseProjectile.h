// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class APlayerCharacter;
UCLASS()
class ABaseProjectile : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UProjectileMovementComponent>movement;
	UPROPERTY(EditAnywhere)TObjectPtr<USphereComponent>sphereHit;
	UPROPERTY(EditAnywhere)TObjectPtr<USkeletalMeshComponent>hand;

	UPROPERTY(EditAnywhere)float knockback;
	UPROPERTY(EditAnywhere)TArray < TObjectPtr<UMaterialInstance>>materialProjectile;
	UPROPERTY(EditAnywhere)UAnimationAsset* animToPlay;


	UPROPERTY()TObjectPtr<APlayerCharacter>shooterRef;
	FVector direction;
	bool hitPlayerBefore = false;
public:
	FORCEINLINE void SetShooterRef(APlayerCharacter* _player) { shooterRef = _player; }
	
public:	
	// Sets default values for this actor's properties
	ABaseProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateInit(APlayerCharacter* _shooter ,ABaseProjectile* _projectile,int _idController);

	UFUNCTION()
	void OnHit( UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateDestroy(ABaseProjectile* _projectile);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateGrab(ABaseProjectile* _projectile, APlayerCharacter* _player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateGrab(ABaseProjectile* _projectile, APlayerCharacter* _player);

	

	void OnlineDestroy();
};
