// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/FinishLine.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Kismet/GameplayStatics.h>
#include <Player/OnlinePlayerState.h>
#include <GameMode/OnlineGameMode.h>

// Sets default values
AFinishLine::AFinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	box = CreateDefaultSubobject<UBoxComponent>("Box");
	finishLine = CreateDefaultSubobject<UStaticMeshComponent>("FinishLine");

	finishLine->SetupAttachment(RootComponent);
	box->SetupAttachment(finishLine);

}

// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
	Super::BeginPlay();
	if (scoreWidgetRef)
	{
		scoreWidget = CreateWidget<UUserWidget>(GetWorld(), scoreWidgetRef);
		if (scoreWidget)
		{
			scoreWidget->AddToViewport();
			scoreWidget->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
	box->OnComponentBeginOverlap.AddDynamic(this, &AFinishLine::OnBoxBeginOverlap); 

	}
}

// Called every frame
void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFinishLine::RoundVictory()
{
}

void AFinishLine::ResetPlayer()
{
}
void AFinishLine::OnBoxBeginOverlap(
	UPrimitiveComponent* OverlappedComp,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	APlayerCharacter* _playerWin = Cast<APlayerCharacter>(OtherActor);
	if (!_playerWin)return;
	AOnlineGameMode* _gm = GetWorld()->GetAuthGameMode<AOnlineGameMode>();
	if (_gm)
	{
		_gm->OnFinishLevel(_playerWin);
		box->OnComponentBeginOverlap.RemoveDynamic(this, &AFinishLine::OnBoxBeginOverlap);
		/*FTimerHandle _timer;
		GetWorld()->GetTimerManager().SetTimer(_timer, this, &AFinishLine::ChangeLevel, 1.5f, false);*/
	}

}

void AFinishLine::ChangeLevel()
{
	GetWorld()->ServerTravel(maps[0].GetLongPackageName() + "?listen");
	UKismetSystemLibrary::PrintString(this, "Change level");

}

