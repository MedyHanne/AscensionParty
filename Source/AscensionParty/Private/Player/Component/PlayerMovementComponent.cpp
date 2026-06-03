// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/PlayerMovementComponent.h"
#include <Player/PlayerCharacter.h>
#include <Components/CapsuleComponent.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UPlayerMovementComponent::UPlayerMovementComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
	// ...
}


// Called when the game starts
void UPlayerMovementComponent::BeginPlay()
{
	Super::BeginPlay();
	customOwner = Cast<APlayerCharacter>(PawnOwner);
	JumpZVelocity = 1500.f;
	AirControl = 1.f;
	FallingLateralFriction = 5.f;
	GravityScale = gravityScaleGround;
	MaxWalkSpeed = speedPlayer;
	
		GetWorld()->GetTimerManager().SetTimer(wallJumpCheckTimer, this, &UPlayerMovementComponent::CheckWallJump, 0.1, true);

	
	// ...

}

void UPlayerMovementComponent::OnMovementModeChanged(EMovementMode PrevMovementMode, uint8 PreviousCustomMode)
{
	Super::OnMovementModeChanged(PrevMovementMode, PreviousCustomMode);
	UpdateGravity();
	

}




// Called every frame
void UPlayerMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	UpdateGravity();
	

}

void UPlayerMovementComponent::Move(float _dir)
{
	
	bool _ToLeft = _dir < 0;
	if (inWallJump || !customOwner)return;
	
	customOwner->AddMovementInput(FVector::RightVector, _dir);
	customOwner->SetActorRotation(FRotator(0.f, _ToLeft ? -90.f : 90.f, 0.f));

}

void UPlayerMovementComponent::WallJump()
{

	if (inWallJump || !customOwner)return;
	FVector _launchVelocity = impactLineTraceWall * wallJumpForce + FVector(0, 0, wallJumpZ);
	pendingLaunchVelocity = _launchVelocity;
	bWantsToWallJump = true;

	//ServerWallJump(_launchVelocity);
	customOwner->LaunchCharacter(_launchVelocity,true,true);

	inWallJump = true;
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, [this]() {inWallJump = false; },0.2f, false);
	Server_ReplicateWallJump(customOwner, _launchVelocity);
}
void UPlayerMovementComponent::Jump()
{
	if (canWallJump)
	{
		WallJump();
	}
	else
	{
		if (customOwner)
			customOwner->Jump();
	}
}
void UPlayerMovementComponent::Dash()
{
	if (isDashing)return;
	isDashing = true;
	GravityScale = 0;
	GroundFriction = 4;
	float _dashDir = customOwner->GetCapsuleComponent()->GetForwardVector().Y*dashPower;
	customOwner->LaunchCharacter(FVector(0, _dashDir, 0), true, false);
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, [this]() {
		
		StopMovementImmediately();
		GravityScale = 3;
		FTimerHandle _timer;
		GetWorld()->GetTimerManager().SetTimer(_timer, [this]() {

			isDashing = false;
			customOwner->CheckDash();
			}, 0.8f, false);
		
		}, 0.1f, false);
	Server_ReplicateDash(customOwner, _dashDir);
}
void UPlayerMovementComponent::ServerWallJump_Implementation(FVector _launchVelocity)
{
	customOwner->LaunchCharacter(_launchVelocity, true, true);
	UKismetSystemLibrary::PrintString(this, "ServerWallJump");

}
void UPlayerMovementComponent::UpdateGravity()
{

	GravityScale = IsFalling() ? gravityScaleFalling : gravityScaleGround;

}

void UPlayerMovementComponent::CheckWallJump()
{
	if (!customOwner)return;
	if (!IsFalling())
	{
		canWallJump = false;
		return;
	}
	UCapsuleComponent* _capsule = customOwner->GetCapsuleComponent();
	FVector _capsuleLoc = _capsule->GetComponentLocation();
	FVector _start = _capsuleLoc - FVector(0, 30, -15);
	FVector _end = _capsuleLoc + _capsule->GetForwardVector() * wallCheckDistance;
	TArray<AActor*> _toIgnores;
	FHitResult _result;
	bool _bHit = UKismetSystemLibrary::SphereTraceSingle(this, _start, _end, 50.0f, ETraceTypeQuery::TraceTypeQuery3, false, _toIgnores, EDrawDebugTrace::ForOneFrame, _result, true);

	canWallJump = _bHit;
	if (canWallJump)
	{
		wallRef = _result.GetActor();

		impactLineTraceWall = FVector(0, _result.ImpactNormal.Y, 0);

	}

}

void UPlayerMovementComponent::CheckWallSlide(float _deltaTime)
{
	if (canWallJump && IsFalling() && Velocity.Z < -wallSlideMaxFallSpeed)
	{
		float _newZ = FMath::FInterpConstantTo(Velocity.Z, -wallSlideMaxFallSpeed, _deltaTime, wallSlideInterpSpeed);
		Velocity.Z = _newZ;
		//UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(Velocity.Z));
	}
}

void UPlayerMovementComponent::Server_ReplicateDash_Implementation(APlayerCharacter* _player, float _dashDir)
{
	Multicast_ReplicateDash(_player, _dashDir);
}
void UPlayerMovementComponent::Multicast_ReplicateDash_Implementation(APlayerCharacter* _player, float _dashDir)
{
	if (!customOwner || customOwner->IsLocallyControlled())return;

	customOwner->LaunchCharacter(FVector(0, _dashDir, 0), true, false);
	FTimerHandle _timer;
	GetWorld()->GetTimerManager().SetTimer(_timer, [this]() {

		StopMovementImmediately();
		GravityScale = 3;
		FTimerHandle _timer;
		GetWorld()->GetTimerManager().SetTimer(_timer, [this]() {

			isDashing = false;
			}, 0.8f, false);

		}, 0.1f, false);
}

void UPlayerMovementComponent::Multicast_ReplicateWallJump_Implementation(APlayerCharacter* _player, FVector _launchVelocity)
{
	if (!_player || _player->IsLocallyControlled())return;
	UKismetSystemLibrary::PrintString(this, "JUMP Multi");

	_player->LaunchCharacter(_launchVelocity, true, true);

}

void UPlayerMovementComponent::Server_ReplicateWallJump_Implementation(APlayerCharacter* _player, FVector _launchVelocity)
{
	Multicast_ReplicateWallJump(_player, _launchVelocity);
}

void UPlayerMovementComponent::Multicast_ReplicateMovement_Implementation(APlayerCharacter* _player, FVector _location, FRotator _rotation)
{
	if (!customOwner ||customOwner->IsLocallyControlled())return;
	_player->SetActorLocation(_location);
	_player->SetActorRotation(_rotation);
}

void UPlayerMovementComponent::Server_ReplicateMovement_Implementation(APlayerCharacter* _player, FVector _location, FRotator _rotation)
{

	Multicast_ReplicateMovement(_player, _location, _rotation);
}



void UPlayerMovementComponent::OnMovementUpdated(float DeltaSeconds, const FVector& OldLocation, const FVector& OldVelocity)
{
	Super::OnMovementUpdated(DeltaSeconds, OldLocation, OldVelocity);

	CheckWallSlide(DeltaSeconds);
}


