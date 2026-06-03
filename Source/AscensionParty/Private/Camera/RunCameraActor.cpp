// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/RunCameraActor.h"
#include "Camera/CameraComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet//GameplayStatics.h>
#include "../Player/PlayerCharacter.h"
#include <GameMode/OnlineGameMode.h>

ARunCameraActor::ARunCameraActor()
{
	PrimaryActorTick.bCanEverTick = true;

	camera = GetCameraComponent();

	lavaMesh = CreateDefaultSubobject<UStaticMeshComponent>("LavaMesh");
	lavaMesh->SetupAttachment(RootComponent);
	lavaMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	
}


 void ARunCameraActor::BeginPlay()
 {
	 Super::BeginPlay();
	 //UKismetSystemLibrary::PrintString(this, "Allo?");

	 if (camera)
		 camera->SetProjectionMode(ECameraProjectionMode::Orthographic);
	 if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	 {
	 FTimerHandle _timer;
	 GetWorld()->GetTimerManager().SetTimer(_timer, this, &ARunCameraActor::CheckPlayers, 1.5f, false);

	 }

	 if (lavaMesh)
	 {
		 float _orthoHeight = camera->OrthoWidth / camera->AspectRatio;
		 float _bottomOffset = -(_orthoHeight * 0.5);

		 lavaMesh->SetRelativeLocation(FVector(200.f, 0.f, _bottomOffset));
		 lavaMesh->SetRelativeScale3D(FVector(0.1f, camera->OrthoWidth / 100.f,0.15));
	 }


 }

 void ARunCameraActor::CheckPlayers()
 {
	  TArray<AActor*> _actors;
	 UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), _actors);

	 for (AActor* _actor : _actors)
	 {
		 if (APlayerCharacter* _player = Cast<APlayerCharacter>(_actor))
		 {
			 players.Add(_player);
		 }
	 }
 }

 void ARunCameraActor::CheckFirst()
 {
	 APlayerCharacter* _top = nullptr;
	 for (APlayerCharacter* _player : players)
	 {
		 if (!_top || _top->GetActorLocation().Z < _player->GetActorLocation().Z)
		 {
			 _top = _player;
		 }

	 }
	 first = _top;
 }

 void ARunCameraActor::CheckDead()
 {
	 float _z = GetActorLocation().Z;
	 float _orthoWidth = camera->OrthoWidth;
	 float _aspectRation = camera->AspectRatio;
	 float _orthoHeight = _orthoWidth/_aspectRation;
	 float _bottomEdge = _z-(_orthoHeight*0.5);
	 for (APlayerCharacter* _player : players)
	 {
		 if (_player->GetActorLocation().Z < _bottomEdge&&!_player->GetIsDead())
		 {
			 _player->Server_Death(_player);

			 AOnlineGameMode* _gm = Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
			 if (_gm)
			 {
				 _gm->RemovePlayer(_player);
				 UKismetSystemLibrary::PrintString(this, "Remove");

			 }
		 }

	 }
 }

 void ARunCameraActor::MoveToFirst(float DeltaSeconds)
 {
	 if (!first)return;
	 FVector _cameraLoc = GetActorLocation();

	 float _firstZ = first->GetActorLocation().Z - cameraToPlayerDistance;
	 float _cameraZ = _cameraLoc.Z;
	 float _result = FMath::FInterpTo(_cameraZ, _firstZ, DeltaSeconds, 10);
	 _cameraLoc.Z = _result;
	 SetActorLocation(_cameraLoc);



 }


 void ARunCameraActor::Tick(float DeltaSeconds)
 {
	 Super::Tick(DeltaSeconds);
	 //UKismetSystemLibrary::PrintString(this, "Allo?");

	 if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	 {
		 CheckFirst();
		 MoveToFirst( DeltaSeconds);
		 CheckDead();
		 Multicast_ReplicateMovement(this, GetActorLocation());

	 }
	
	
}

 void ARunCameraActor::Multicast_ReplicateMovement_Implementation(ACameraActor* _camera, FVector _location)
 {
	 _camera->SetActorLocation(_location);
 }
