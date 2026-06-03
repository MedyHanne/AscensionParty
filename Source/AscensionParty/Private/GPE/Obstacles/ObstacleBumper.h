// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Obstacle.h"
#include "ObstacleBumper.generated.h"

class APlayerCharacter ;
UCLASS()
class AObstacleBumper : public AObstacle
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObstacleBumper();
	float launchZ = 2000;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void OnWalkIn(UPrimitiveComponent* _overlappedComponent, AActor* _otherActor, UPrimitiveComponent* _otherComp, int32 _otherBodyIndex, bool _bFromSweep, const FHitResult& _sweepResult);

	
};
