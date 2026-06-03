// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include <InputMappingContext.h>
#include <InputActionValue.h>
#include "CommonInputTypeEnum.h"
#include <Components/WidgetComponent.h>
#include "PlayerCharacter.generated.h"

class UInputDataAsset;
class UPlayerMovementComponent;
class UArrowComponent;
class UProjectileComponent;
class UItemComponent;
UCLASS()
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, Category = "Input") TObjectPtr<UInputDataAsset> inputs;
	UPROPERTY(EditAnywhere)TObjectPtr<UPlayerMovementComponent> movement;
	UPROPERTY(EditAnywhere)TObjectPtr<UProjectileComponent> shoot;
	UPROPERTY(EditAnywhere)TObjectPtr<UArrowComponent> arrow;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent>powerAura;
	
	UPROPERTY(EditAnywhere, Category = "Dash")TObjectPtr<UWidgetComponent>dashIndicatorWidget;
	UPROPERTY(EditAnywhere, Category = "Projectile")TObjectPtr<UWidgetComponent>barMunition;
	UPROPERTY(EditAnywhere, Category = "Projectile")TObjectPtr<UWidgetComponent>aimWidget;
	UPROPERTY(EditAnywhere, Category = "Item")TObjectPtr<UWidgetComponent>itemWidget;
	UPROPERTY(EditAnywhere, Category = "Item")TObjectPtr<UWidgetComponent>barDelayWidget;
	UPROPERTY(EditAnywhere, Category = "Item")TObjectPtr<UItemComponent>item;


	UPROPERTY(EditAnywhere)TArray < TObjectPtr<UMaterialInstance>>materialCharacter;
	UPROPERTY(EditAnywhere)TArray < TObjectPtr<UMaterialInstance>>materialCharacter2;
	UPROPERTY(EditAnywhere)TArray < TObjectPtr<UMaterialInstance>>overlayMaterial;
	
	UPROPERTY(VisibleAnywhere)bool bUseGamepad;
	bool isDead = false;
	bool hitProjectile = false;
	bool isKnockback = false;


protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)int idController = 0;
public:
	FORCEINLINE bool GetIsKnockback() { return isKnockback; }
	FORCEINLINE bool GetIsDead() { return isDead; }
	FORCEINLINE bool GetHitProjectile() { return hitProjectile; }
	FORCEINLINE int GetIdController() { return idController; }
	FORCEINLINE UArrowComponent* GetAim() { return arrow; }
	FORCEINLINE UWidgetComponent* GetAimWidget() { return aimWidget; }
	FORCEINLINE UPlayerMovementComponent* GetMovement() { return movement; }
	FORCEINLINE UWidgetComponent* GetBarMunition() { return barMunition; }
	FORCEINLINE UItemComponent* GetItemComponent() { return item; }
	FORCEINLINE UStaticMeshComponent* GetPowerAura() { return powerAura; }
	FORCEINLINE void SetHitProjectile(bool _hitProjectile) {  hitProjectile= _hitProjectile; }
	FORCEINLINE void SetIsKnockback(bool _isKnockback) { isKnockback = _isKnockback; }
	
public:
	// Sets default values for this character's properties
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void InitInput();
	virtual void OnRep_Controller()override;
	virtual void PossessedBy(AController* NewController)override;
	void OnInputDeviceChanged(ECommonInputType InputType);
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void UpdateAimDirection();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION()void Move(const FInputActionValue& _value);
	UFUNCTION()void InputJump(const FInputActionValue& _value);
	UFUNCTION()void InputStopJump(const FInputActionValue& _value);
	UFUNCTION()void Dash(const FInputActionValue& _value);
	UFUNCTION()void Shoot(const FInputActionValue& _value);
	UFUNCTION()void UseItem(const FInputActionValue& _value);
	//UFUNCTION()void MouseLook(const FInputActionValue& _value);


	UFUNCTION(BlueprintCallable,NetMulticast, Reliable)
	void  Multicast_ChangeColor(APlayerCharacter* _player,int _idController);

	void CheckDash();
	void Death();
	UFUNCTION(Server, Reliable)
	void Server_Death(APlayerCharacter* _player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Death(APlayerCharacter* _player);

	UFUNCTION(Server, Reliable)
	void Server_ReplicateCheckDash(APlayerCharacter* _player, bool _isDashing);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateCheckDash(APlayerCharacter* _player, bool _isDashing);

	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ReplicateEnableInput(APlayerCharacter* _player, bool _enable);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ReplicateHideWidget(APlayerCharacter* _player, bool _enable);
	
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ReplicateUpdateCount(APlayerCharacter* _player,int _text);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ReplicateShowScore(APlayerCharacter* _player);
	UFUNCTION(NetMulticast, Reliable, BlueprintCallable)
	void Multicast_ReplicateStartPlatform(APlayerCharacter* _player);
};
