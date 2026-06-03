// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include <Components/SplineComponent.h>
#include <Components/SphereComponent.h>
#include <Components/SplineMeshComponent.h>
#include "ItemTeleport.generated.h"

class APlayerCharacter;
UCLASS()
class AItemTeleport : public AItem
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<USplineComponent> splineChain;
	UPROPERTY(EditAnywhere)TObjectPtr<USplineMeshComponent> splineMesh;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> hitSphere;
	UPROPERTY(EditAnywhere)TObjectPtr<USphereComponent> sphereOverlap;
	

	UPROPERTY()TArray<TObjectPtr<APlayerCharacter>>inRangePlayers;
	UPROPERTY()TObjectPtr<APlayerCharacter> nearestPlayer;
	UPROPERTY()FTimerHandle endTimer;
	UPROPERTY()FTimerHandle updatePlayerTimer;


	UPROPERTY(EditAnywhere)TObjectPtr<UMaterialInstance>orangeMat;
	UPROPERTY(EditAnywhere)TObjectPtr<UMaterialInstance>redMat;
	UPROPERTY(EditAnywhere)float teleportDelay=0.5;
	UPROPERTY(EditAnywhere)float radiusmax=950;

	float updateRate = 0.001;

	float progress = 0;

public:	
	// Sets default values for this actor's properties
	AItemTeleport();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Use()override;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdatePlayers();
	void UpdateSpline();
	void UpdateSphere();
	void Update();
	void Finish();
	UFUNCTION() void OnHitSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()void OnHitSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION()void OnSphereOverlapEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	UFUNCTION(Server, Reliable)
	void Server_ReplicateUseTeleport(AItemTeleport* _item);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUseTeleport(AItemTeleport* _item);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUpdateSpline(AItemTeleport* _item, APlayerCharacter* _player,APlayerCharacter* _target);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUpdateSphere(AItemTeleport* _item,float _time);

};
