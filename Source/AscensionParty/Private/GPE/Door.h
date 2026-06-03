// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../LeverActivableInterface.h"
#include "Door.generated.h"

UCLASS()
class ADoor : public AActor,public ILeverActivableInterface
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> center;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> door;
	UPROPERTY(EditAnywhere) FRotator finalRotation;
	UPROPERTY()FTimerHandle rotateTimer;
	
public:	
	// Sets default values for this actor's properties
	ADoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


	// Inherited via ILeverActivableInterface
	virtual void Activate_Implementation() override;
	void Rotate() ;
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ReplicateRotate(ADoor* _door, FRotator _rotation);

};
