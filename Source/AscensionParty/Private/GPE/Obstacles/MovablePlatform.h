// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/InterpToMovementComponent.h>
#include "MovablePlatform.generated.h"

UCLASS()
class AMovablePlatform : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>platform;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)TObjectPtr<UInterpToMovementComponent>movement;
	UPROPERTY(EditAnywhere)float time;
	UPROPERTY(EditAnywhere)TArray<FInterpControlPoint> wayPoints;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)bool canActivate = false;
	
public:	
	// Sets default values for this actor's properties
	AMovablePlatform();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform) override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void StartMovement();
	UFUNCTION(BlueprintCallable)void Activate();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Synchronize(AMovablePlatform* _platform,FVector _position);
	UFUNCTION(Server, Reliable)
	void Server_Synchronize(AMovablePlatform* _platform);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateStart(AMovablePlatform* _platform);
	UFUNCTION(Server, Reliable)
	void Server_ReplicateStart(AMovablePlatform* _platform);
};
