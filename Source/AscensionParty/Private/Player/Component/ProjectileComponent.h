// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h" 
#include <Kismet/KismetSystemLibrary.h>

#include "ProjectileComponent.generated.h"

class APlayerCharacter;
class UArrowComponent;
class ABaseProjectile;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UProjectileComponent : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)TObjectPtr<APlayerCharacter> customOwner;
	UPROPERTY(EditAnywhere)TSubclassOf<ABaseProjectile>projectile;
	
	UPROPERTY(EditAnywhere)float munitionLeft = 100;
	UPROPERTY(EditAnywhere)float munitionUse = 20;
	UPROPERTY(EditAnywhere)float maxMunition = 100;
	UPROPERTY(EditAnywhere)float munitionRegeneration = 20;

	FTimerHandle regenTimer;
	UPROPERTY(EditAnywhere)float regenRate = 2.0f;
	FRotator direction;
public:
	FORCEINLINE void SetDirection(FRotator _dir) { direction = _dir; }
public:	
	// Sets default values for this component's properties
	UProjectileComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(Server, Reliable)
	void Server_CreateProjectile(TSubclassOf<ABaseProjectile> _projectile, FVector _location, FRotator _rotation);

	void Shoot();
	void UpdateWidget();
	void Regenerate();
		
};
