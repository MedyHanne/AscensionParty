// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "RunCameraActor.generated.h"

/**
 * 
 */
class UCameraComponent;
class APlayerCharacter;
UCLASS()
class ARunCameraActor : public ACameraActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>lavaMesh;
	UPROPERTY(EditAnywhere)TObjectPtr<UCameraComponent>camera;
	UPROPERTY(EditAnywhere)TArray<TObjectPtr<APlayerCharacter>>players;
	UPROPERTY(EditAnywhere)TObjectPtr<APlayerCharacter>first;

	UPROPERTY(EditAnywhere)float cameraToPlayerDistance;


public:
	ARunCameraActor();

protected:
	virtual void BeginPlay()override;

	virtual void Tick(float DeltaSeconds);

	UFUNCTION(BlueprintCallable)
	void CheckPlayers();
	void CheckFirst();
	void CheckDead();
	void MoveToFirst(float DeltaSeconds);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateMovement(ACameraActor* _camera, FVector _location);


};
