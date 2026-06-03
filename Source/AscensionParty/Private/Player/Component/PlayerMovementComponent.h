// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "PlayerMovementComponent.generated.h"

class APlayerCharacter;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UPlayerMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)TObjectPtr<APlayerCharacter> customOwner;
	UPROPERTY(EditAnywhere)float speedPlayer = 800.0f;
	UPROPERTY(EditAnywhere)float gravityScaleGround = 1;
	UPROPERTY(EditAnywhere)float gravityScaleWall =0.75;
	UPROPERTY(EditAnywhere)float gravityScaleFalling = 3;

#pragma region WallJump

	UPROPERTY()TObjectPtr<AActor>wallRef;
	UPROPERTY(EditAnywhere)float wallCheckDistance = 60;
	UPROPERTY(EditAnywhere)float wallJumpForce = 300;
	UPROPERTY(EditAnywhere)float wallJumpZ = 140;
	UPROPERTY(EditAnywhere)float wallSlideMaxFallSpeed = 150.0f;
	UPROPERTY(EditAnywhere)float wallSlideInterpSpeed = 400.f;
	UPROPERTY()FTimerHandle wallJumpCheckTimer;
	FVector impactLineTraceWall;
	bool canWallJump = false;
	bool inWallJump = false;
public:

	bool bWantsToWallJump =false;
	FVector pendingLaunchVelocity;
#pragma endregion
	
#pragma region Dash
private:
	UPROPERTY(EditAnywhere)float dashPower = 8000.0f;
	bool isDashing = false;



#pragma endregion


	//float moveDir = 0.f;

public:
	/*FORCEINLINE float GetMoveDir() { return moveDir; }
	FORCEINLINE void SetMoveDir(float _value) {  moveDir=_value; }*/
	FORCEINLINE bool IsDashing() { return isDashing; }
	FORCEINLINE void SetIsDashing(bool _isDashing) {  isDashing= _isDashing; }
public:	
	// Sets default values for this component's properties
	UPlayerMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)override;
	//virtual void UpdateFromCompressedFlags(uint8 Flags)override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void Move(float _dir);
	void Jump();
	void Dash();
	UFUNCTION(Server,Reliable)
	void ServerWallJump(FVector _launchVelocity);
	void WallJump();
	void UpdateGravity();
	void CheckWallJump();
	void CheckWallSlide(float _deltaTime);
	UFUNCTION(Server,Reliable)
	void Server_ReplicateMovement(APlayerCharacter* _player, FVector _location, FRotator _rotation);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ReplicateMovement(APlayerCharacter* _player, FVector _location, FRotator _rotation);
	UFUNCTION(Server,Reliable)
	void Server_ReplicateWallJump(APlayerCharacter* _player, FVector _launchVelocity);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ReplicateWallJump(APlayerCharacter* _player, FVector _launchVelocity);
	UFUNCTION(Server,Reliable)
	void Server_ReplicateDash(APlayerCharacter* _player, float _dashDir);
	UFUNCTION(NetMulticast,Reliable)
	void Multicast_ReplicateDash(APlayerCharacter* _player, float _dashDir);
	
	virtual void OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity) override;
	
};

