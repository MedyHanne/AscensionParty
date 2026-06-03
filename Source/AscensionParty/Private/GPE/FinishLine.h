// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include <Components/BoxComponent.h>
#include  "../Player/PlayerCharacter.h"
#include "FinishLine.generated.h"

UCLASS()
class AFinishLine : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)TObjectPtr<UBoxComponent> box;
	UPROPERTY(EditAnywhere)TObjectPtr<UStaticMeshComponent> finishLine;
	UPROPERTY(EditAnywhere)TSubclassOf<UUserWidget>scoreWidgetRef;
	UPROPERTY(EditAnywhere)TObjectPtr<UUserWidget>scoreWidget;
	UPROPERTY()TArray<TObjectPtr<APlayerCharacter>> players;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSoftObjectPtr<UWorld>> maps;


	
public:	
	// Sets default values for this actor's properties
	AFinishLine();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void RoundVictory();
	void ResetPlayer();
	UFUNCTION()
	void OnBoxBeginOverlap(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);
	void ChangeLevel();

};
