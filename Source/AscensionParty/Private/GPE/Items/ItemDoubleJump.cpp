// Fill out your copyright notice in the Description page of Project Settings.


#include "GPE/Items/ItemDoubleJump.h"
#include <Player/PlayerCharacter.h>
#include<Player/Component/ItemComponent.h>
#include <Kismet/KismetSystemLibrary.h>


// Sets default values
AItemDoubleJump::AItemDoubleJump()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AItemDoubleJump::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemDoubleJump::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemDoubleJump::Use()
{
	if (use)return;
	//refUser->JumpMaxCount++;
	UStaticMeshComponent* _powerAura = refUser->GetPowerAura();
	previousMat=Cast<UMaterialInstance>(_powerAura->GetMaterial(0));
	/*_powerAura->SetMaterial(0, auraMat);
	_powerAura->SetVisibility(true);*/
	UKismetSystemLibrary::PrintString(this, "JE PASSE");
	use = true;
	GetWorld()->GetTimerManager().SetTimer(endTimer, this, &AItemDoubleJump::EndUse, duration, false);
	GetWorld()->GetTimerManager().SetTimer(timer, this, &AItemDoubleJump::UpdateBar, 0.1, true);
	if (UWidgetComponent* _widgetComp = refUser->GetItemComponent()->GetBarDelayWidget())
	{
		_widgetComp->SetVisibility(true);
		UUserWidget* _widget = Cast<UUserWidget>(_widgetComp->GetWidget());
		if (_widget)
		{
			FProperty* _prop = _widget->GetClass()->FindPropertyByName(TEXT("MaxDelay"));
			if (FDoubleProperty* _floatProp = CastField<FDoubleProperty>(_prop))
			{
				
				_floatProp->SetPropertyValue_InContainer(_widget, duration);

			}
		}

	}
	Server_ReplicateUseJump(this, refUser);
	/*if (HasAuthority())
		Multicast_ReplicateUseJump(this, refUser);
	else*/
	//Super::Use();
	
}

void AItemDoubleJump::Server_ReplicateUseJump_Implementation(AItemDoubleJump* _item, APlayerCharacter* _player)
{
	//UKismetSystemLibrary::PrintString(_item, "UseJump");
	/*GetWorld()->GetTimerManager().SetTimer(_item->endTimer, _item, &AItemDoubleJump::EndUse, _item->duration, false);
	GetWorld()->GetTimerManager().SetTimer(_item->timer, _item, &AItemDoubleJump::UpdateBar, 0.1, true);*/
	Multicast_ReplicateUseJump(_item, _player);
}

void AItemDoubleJump::Multicast_ReplicateUseJump_Implementation(AItemDoubleJump* _item, APlayerCharacter* _player)
{
	_player->JumpMaxCount++;

	UKismetSystemLibrary::PrintString(_item, "UseJumpClient");

	UStaticMeshComponent* _powerAura = _player->GetPowerAura();
	if (!_powerAura)
		UKismetSystemLibrary::PrintString(_item, "Pas D'aura");
	if (!_item->auraMat)
		UKismetSystemLibrary::PrintString(_item, "Pas De Mat");

	_powerAura->SetMaterial(0, _item->auraMat);
	_powerAura->SetVisibility(true);
	if (UWidgetComponent* _widgetComp = _player->GetItemComponent()->GetBarDelayWidget())
	{
		_widgetComp->SetVisibility(true);
		UUserWidget* _widget = Cast<UUserWidget>(_widgetComp->GetWidget());
		if (_widget)
		{
			FProperty* _prop = _widget->GetClass()->FindPropertyByName(TEXT("MaxDelay"));
			if (FDoubleProperty* _floatProp = CastField<FDoubleProperty>(_prop))
			{
				UKismetSystemLibrary::PrintString(_item, "MaxDelay");

				_floatProp->SetPropertyValue_InContainer(_widget, _item->duration);

			}
		}

	}
}

void AItemDoubleJump::Server_ReplicateUpdateBar_Implementation(AItemDoubleJump* _item, APlayerCharacter* _player, float _timeRemaining)
{
	Multicast_ReplicateUpdateBar(_item, _player, _timeRemaining);
}

void AItemDoubleJump::Multicast_ReplicateUpdateBar_Implementation(AItemDoubleJump* _item, APlayerCharacter* _player, float _timeRemaining)
{
	if (UWidgetComponent* _widgetComp = _player->GetItemComponent()->GetBarDelayWidget())
	{
		_widgetComp->SetVisibility(true);
		UUserWidget* _widget = Cast<UUserWidget>(_widgetComp->GetWidget());
		if (_widget)
		{
			FProperty* _prop = _widget->GetClass()->FindPropertyByName(TEXT("CurrentDelay"));
			if (FDoubleProperty* _floatProp = CastField<FDoubleProperty>(_prop))
			{
				_floatProp->SetPropertyValue_InContainer(_widget, _timeRemaining);
				//UKismetSystemLibrary::PrintString(_item, "CurrentDelay");

			}
		}

	}
	else
	{
		
		UKismetSystemLibrary::PrintString(_item, "Pas De Bar ");
	}
}



void AItemDoubleJump::Server_ReplicateEndUse_Implementation(AItemDoubleJump* _item, APlayerCharacter* _player)
{
	Multicast_ReplicateEndUse(_item, _player);
}

void AItemDoubleJump::Multicast_ReplicateEndUse_Implementation(AItemDoubleJump* _item, APlayerCharacter* _player)
{
	UStaticMeshComponent* _powerAura = _player->GetPowerAura();

	_powerAura->SetMaterial(0, _item->previousMat);
	_powerAura->SetVisibility(false);
	if (UWidgetComponent* _widgetComp = _player->GetItemComponent()->GetBarDelayWidget())
	{
		_widgetComp->SetVisibility(false);
	}
}

void AItemDoubleJump::UpdateBar()
{
	//UKismetSystemLibrary::PrintString(this, "UpdateBar");

	float _timeRemaining = GetWorld()->GetTimerManager().GetTimerRemaining(endTimer);
	Server_ReplicateUpdateBar(this, refUser, _timeRemaining);
	
	
	
}

void AItemDoubleJump::EndUse()
{
	refUser->JumpMaxCount--;
	Server_ReplicateEndUse(this, refUser);
	Super::Use();
	
}

