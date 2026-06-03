// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include <Components/WidgetComponent.h>
#include "ItemComponent.generated.h"

class AItem;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UItemComponent : public UActorComponent
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)TObjectPtr<AItem>currentItem;
	UPROPERTY(EditAnywhere, Category = "Item")TObjectPtr<UWidgetComponent>itemWidget;
	UPROPERTY(EditAnywhere, Category = "Item")TObjectPtr<UWidgetComponent>barDelayWidget;

public:

	FORCEINLINE AItem* GetItem() { return currentItem; }
	FORCEINLINE UWidgetComponent* GetItemWidget() { return itemWidget; }
	FORCEINLINE UWidgetComponent* GetBarDelayWidget() { return barDelayWidget; }
	FORCEINLINE void SetItemWidget(UWidgetComponent* _widget) {  itemWidget=_widget; }
	FORCEINLINE void SetBarDelayWidget(UWidgetComponent* _barDelayWidget) { barDelayWidget =_barDelayWidget; }
	FORCEINLINE void SetItem(AItem* _item) { currentItem = _item; }
public:	
	// Sets default values for this component's properties
	UItemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void UseItem();

	UFUNCTION(Server, Reliable)
	void Server_ReplicateUseItem(UItemComponent* _player);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_ReplicateUseItem(UItemComponent* _player);
		
};
