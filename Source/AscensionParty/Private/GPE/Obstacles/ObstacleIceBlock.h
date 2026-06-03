// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "ObstacleIceBlock.generated.h"
class APlayerCharacter;
UCLASS()
class AObstacleIceBlock : public AObstacle
{
	GENERATED_BODY() 
	UPROPERTY(EditAnywhere) float groundFriction = 0;
	UPROPERTY(EditAnywhere) float brakingDecelerationWalking = 350;
	UPROPERTY(EditAnywhere) float groundFrictionNormal = 8;
	UPROPERTY(EditAnywhere) float brakingDecelerationWalkingNormal = 2000;
	
public:	
	// Sets default values for this actor's properties
	AObstacleIceBlock();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWalkIn(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION()
	void OnWalkOut(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex);


	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateOnWalkIn(APlayerCharacter* _player, float _groundFriction, float _brakingDecelerationWalking);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateOnWalkOut(APlayerCharacter* _player, float _groundFriction, float _brakingDecelerationWalking);
};
