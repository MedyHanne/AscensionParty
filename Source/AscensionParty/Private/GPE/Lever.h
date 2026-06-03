// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacles/Obstacle.h"
#include <LeverActivableInterface.h>
#include "Lever.generated.h"

UCLASS()
class ALever : public AObstacle
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<USceneComponent>pivot;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>lever;

	
	UPROPERTY(EditAnywhere, meta = (MustImplement = "LeverActivableInterface"))TArray<TObjectPtr<AActor>> activables;
	
public:	
	// Sets default values for this actor's properties
	ALever();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(BlueprintImplementableEvent)void Rotate();

	UFUNCTION() void OnWalkIn(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateRotate(ALever* _lever, FRotator _rotation);
};
