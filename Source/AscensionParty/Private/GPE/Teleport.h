// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Teleport.generated.h"

UCLASS()
class ATeleport : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>teleport;
	UPROPERTY(EditAnywhere)TObjectPtr<USceneComponent>spawn;
	UPROPERTY(EditAnywhere)TObjectPtr<ATeleport>teleportRef;
	
public:	
	// Sets default values for this actor's properties
	ATeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION() void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
};
