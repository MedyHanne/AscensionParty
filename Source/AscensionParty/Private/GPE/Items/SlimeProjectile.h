// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlimeProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class APlayerCharacter;
class ASlime;
UCLASS()
class ASlimeProjectile : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UProjectileMovementComponent>movement;
	UPROPERTY(EditAnywhere)TObjectPtr<USphereComponent>sphereHit;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>body;
	UPROPERTY(EditAnywhere)TSubclassOf<ASlime> zone;
	UPROPERTY()TObjectPtr<APlayerCharacter>shooterRef;
public:

public:	
	// Sets default values for this actor's properties
	ASlimeProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, FVector NormalImpulse,
		const FHitResult& Hit);
	void OnHitTest(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	
public:	
	void SetShooterRef(APlayerCharacter* _player);
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Spawn(AActor* _hitActor) ;

};
