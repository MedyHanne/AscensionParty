// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/ItemComponent.h"
#include <Player/PlayerCharacter.h>
#include <GPE/Items/Item.h>
#include <Kismet/KismetSystemLibrary.h>

// Sets default values for this component's properties
UItemComponent::UItemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UItemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UItemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UItemComponent::UseItem()
{
	if (!currentItem)
	{
		UKismetSystemLibrary::PrintString(this, "Pas d'item");
		return;
	}
	currentItem->Use();
	/*if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
		currentItem->Multicast_ReplicateUse(currentItem, Cast<APlayerCharacter>(GetOwner()));
	else
	{
		currentItem->Server_ReplicateUse(currentItem, Cast<APlayerCharacter>(GetOwner()));
	}*/
	Server_ReplicateUseItem(this);
}

void UItemComponent::Multicast_ReplicateUseItem_Implementation(UItemComponent* _player)
{
	_player->itemWidget->SetVisibility(false);
}

void UItemComponent::Server_ReplicateUseItem_Implementation(UItemComponent* _player)
{
	Multicast_ReplicateUseItem(_player);
}

