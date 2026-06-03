// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include "Slime.generated.h"

class APlayerCharacter;
UCLASS()
class ASlime : public AActor
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>slimeBody;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>cylinder;
	UPROPERTY(EditAnywhere)TObjectPtr<UBoxComponent>box;
	UPROPERTY(EditAnywhere)TObjectPtr<APlayerCharacter>shooterRef;
	UPROPERTY(EditAnywhere)TObjectPtr<AActor>hitActor;
	UPROPERTY(EditAnywhere)TArray < TObjectPtr<UMaterialInstance>>materialOverlay;

public:
	FORCEINLINE TObjectPtr<UBoxComponent> GetBox() { return box; }
public:	
	// Sets default values for this actor's properties
	ASlime();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	UFUNCTION() void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	void SetShooter(APlayerCharacter* _player);
	void SetHitActor(AActor* _actor);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateSetShooter(ASlime* _slime, APlayerCharacter* _player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateSetHitActor(ASlime* _slime, AActor* _actor);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateOverlap(APlayerCharacter* _player, float _scustomTimeDilation);

};
