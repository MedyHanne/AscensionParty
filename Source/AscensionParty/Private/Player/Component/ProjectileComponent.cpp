// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Component/ProjectileComponent.h"
#include "../PlayerCharacter.h"
#include "../Projectiles/BaseProjectile.h"
#include <Components/ArrowComponent.h>
#include <Kismet/GameplayStatics.h>

// Sets default values for this component's properties
UProjectileComponent::UProjectileComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void UProjectileComponent::BeginPlay()
{
	Super::BeginPlay();
	customOwner = Cast<APlayerCharacter>(GetOwner());
	if(customOwner->IsLocallyControlled())
		GetWorld()->GetTimerManager().SetTimer(regenTimer, this, &UProjectileComponent::Regenerate, regenRate, true);
	
	// ...
	
}


// Called every frame
void UProjectileComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UProjectileComponent::Server_CreateProjectile_Implementation(TSubclassOf<ABaseProjectile> _projectile, FVector _location, FRotator _rotation)
{
	ABaseProjectile* _projectileBase = GetWorld()->SpawnActorDeferred<ABaseProjectile>(_projectile, FTransform(_rotation, _location));
	if (_projectileBase)
	{
		_projectileBase->SetShooterRef(customOwner);
	}
	UGameplayStatics::FinishSpawningActor(_projectileBase, FTransform(_rotation, _location));
}

void UProjectileComponent::Shoot()
{

	if (munitionLeft < munitionUse ||!customOwner ||!customOwner->GetAim() || !projectile)
	{
		if(munitionLeft < munitionUse)
			UKismetSystemLibrary::PrintString(this, "pas mun");
		if(!customOwner)
			UKismetSystemLibrary::PrintString(this, "owner");
		else if(!customOwner->GetAim())
			UKismetSystemLibrary::PrintString(this, "pas aim");
		if(!projectile)
			UKismetSystemLibrary::PrintString(this, "pas proj");

		return;
	}
	UArrowComponent* _aim = customOwner->GetAim();
	munitionLeft -= munitionUse;
	GetWorld()->GetTimerManager().SetTimer(regenTimer, this, &UProjectileComponent::Regenerate, regenRate, true);
	UWidgetComponent* _widget = customOwner->GetAimWidget();

	if (!customOwner->HasAuthority())
	{
		Server_CreateProjectile(projectile, _aim->GetComponentLocation(),direction);

	}
	else
	{
		//UKismetSystemLibrary::PrintString(this, _aim->GetComponentRotation().ToString());
		//GetWorld()->SpawnActor<ABaseProjectile>(projectile, aim->GetComponentLocation(), aim->GetComponentRotation());
		//ABaseProjectile* _projectile = GetWorld()->SpawnActorDeferred<ABaseProjectile>(projectile,FTransform(_aim->GetComponentRotation(), _aim->GetComponentLocation()));
		ABaseProjectile* _projectileBase = GetWorld()->SpawnActorDeferred<ABaseProjectile>(projectile, FTransform(direction, _aim->GetComponentLocation()));
		if (_projectileBase)
		{
			_projectileBase->SetShooterRef(customOwner);
		}
		UGameplayStatics::FinishSpawningActor(_projectileBase, FTransform(direction, _aim->GetComponentLocation()));
	}

	UpdateWidget();

}
void UProjectileComponent::UpdateWidget()
{
	UWidgetComponent* _barMunition = customOwner->GetBarMunition();
	UUserWidget* _widget = _barMunition->GetWidget();

	FProperty* _property = _widget->GetClass()->FindPropertyByName("CurrentMunition");
		if (_property)
		{
			double* _valuePtr = _property->ContainerPtrToValuePtr<double>(_widget);
			if (_valuePtr)
			{

				UKismetSystemLibrary::PrintString(this, FString::SanitizeFloat(munitionLeft));
				*_valuePtr = (double)munitionLeft;
			}
		}
	// ...

}
void UProjectileComponent::Regenerate()
{
	//UKismetSystemLibrary::PrintString(this, "regen");

	if (munitionLeft < maxMunition)
	{
		munitionLeft += munitionRegeneration;
		munitionLeft = FMath::Min(munitionLeft, maxMunition);
		UpdateWidget();
	}

	if (munitionLeft == maxMunition)
	{
		GetWorld()->GetTimerManager().ClearTimer(regenTimer);

	}
}

