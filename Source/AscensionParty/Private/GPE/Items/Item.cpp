// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/Item.h"
#include <Player/PlayerCharacter.h>
#include <Player/Component/ItemComponent.h>
#include "SpawnItem.h"
#include <Kismet/KismetSystemLibrary.h>

// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>("Root");
	body = CreateDefaultSubobject<UStaticMeshComponent>("Body");
	plane = CreateDefaultSubobject<UStaticMeshComponent>("Plane");

	body->SetupAttachment(RootComponent);
	plane->SetupAttachment(RootComponent);
	bReplicates = true;
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	UKismetSystemLibrary::PrintString(this,
		FString::Printf(TEXT("Collision enabled: %d"),
			(int)body->GetCollisionEnabled()),
		true, true, FLinearColor::Green, 10);
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		UKismetSystemLibrary::PrintString(this, "Init", true, true, FLinearColor::Red, 20);
		body->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnHit);
	}
}

// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
	
void AItem::OnHit(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepHitResult)
{

	APlayerCharacter* _player = Cast<APlayerCharacter>(OtherActor);
	if (!_player || _player->GetItemComponent()->GetItem())
	{
		if (_player&&_player->GetItemComponent()->GetItem())
			UKismetSystemLibrary::PrintString(this, "A un Item");
		if (!_player)
			UKismetSystemLibrary::PrintString(this, OtherActor->GetFullName());
		return;
	}
	if (!_player->GetController())
	{
		UKismetSystemLibrary::PrintString(this, "Pas de Controller", true, true, FLinearColor::Red, 5);

	}
	SetOwner(_player->GetController());

	Multicast_ReplicateOnHit(this, _player);
	if (spawner)
		spawner->OnRespawn();
}

void AItem::Server_ReplicateOnHit_Implementation(AItem* _item, APlayerCharacter* _player)
{
	Multicast_ReplicateOnHit(_item, _player);
}

void AItem::Use()
{
	UKismetSystemLibrary::PrintString(this, "Use");

	
	Server_ReplicateUse(this, refUser);
	
}

void AItem::Server_ReplicateUse_Implementation(AItem* _item, APlayerCharacter* _player)
{
	Multicast_ReplicateUse(_item, _player);
}


void AItem::Multicast_ReplicateUse_Implementation(AItem* _item, APlayerCharacter* _player)
{
	UKismetSystemLibrary::PrintString(_item, "Replicate");

	_player->GetItemComponent()->SetItem(nullptr);
	_player->GetItemComponent()->GetItemWidget()->SetVisibility(false);
	_item->Destroy();
}

void AItem::Multicast_ReplicateOnHit_Implementation(AItem* _item, APlayerCharacter* _player)
{
	UKismetSystemLibrary::PrintString(_item, "OnHit",true,true,FLinearColor::Red,20);
	_item->refUser = _player;
	
	UItemComponent* _itemComp = _item->refUser->GetItemComponent();
	_itemComp->SetItem(_item);
	//_item->body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	body->OnComponentBeginOverlap.RemoveDynamic(this, &AItem::OnHit);

	_item->plane->SetVisibility(false);
	_item->body->SetVisibility(false);
	if (UWidgetComponent* _widgetComp = _itemComp->GetItemWidget())
	{
		_widgetComp->SetVisibility(true);
		UUserWidget* _widget = Cast<UUserWidget>(_widgetComp->GetWidget());
		if (_widget)
		{
			FProperty* _prop = _widget->GetClass()->FindPropertyByName(TEXT("Index_Image"));
			if (FIntProperty* _intProp = CastField<FIntProperty>(_prop))
				_intProp->SetPropertyValue_InContainer(_widget, indexTexture);
		}

	}

}


