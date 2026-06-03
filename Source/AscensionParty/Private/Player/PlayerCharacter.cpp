// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/PlayerCharacter.h"
#include "Input/InputDataAsset.h"
#include <EnhancedInputComponent.h>
#include <EnhancedInputSubsystems.h>
#include <Net/UnrealNetwork.h>
#include "GameFramework/CharacterMovementComponent.h"
#include "Component/PlayerMovementComponent.h"
#include <Kismet/KismetSystemLibrary.h>
#include <Components/CapsuleComponent.h>
#include <Components/Image.h>
#include <Components/ArrowComponent.h>
#include "Component/ProjectileComponent.h"
#include <CommonInputSubsystem.h>
#include "Component/ItemComponent.h"
#include <GameMode/OnlineGameMode.h>
#include "GameFramework/PlayerController.h"
#include <UI/HUD/GameHUD.h>
#include <Kismet/GameplayStatics.h>
#include<GPE/Obstacles/MovablePlatform.h>
// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	arrow = CreateDefaultSubobject<UArrowComponent>("Aim");
	shoot = CreateDefaultSubobject<UProjectileComponent>("Shoot");
	movement = Cast<UPlayerMovementComponent>(GetMovementComponent());
	dashIndicatorWidget = CreateDefaultSubobject<UWidgetComponent>("DashWidget");
	aimWidget = CreateDefaultSubobject<UWidgetComponent>("AimWidget");
	barMunition = CreateDefaultSubobject<UWidgetComponent>("MunitionWidget");
	itemWidget = CreateDefaultSubobject<UWidgetComponent>("ItemWidget");
	barDelayWidget = CreateDefaultSubobject<UWidgetComponent>("BarDelayWidget");
	item = CreateDefaultSubobject<UItemComponent>("ItemComponent");
	powerAura = CreateDefaultSubobject<UStaticMeshComponent>("PowerAura");

	dashIndicatorWidget->SetupAttachment(GetCapsuleComponent());
	barMunition->SetupAttachment(GetCapsuleComponent());
	arrow->SetupAttachment(GetCapsuleComponent());
	itemWidget->SetupAttachment(GetCapsuleComponent());
	barDelayWidget->SetupAttachment(GetCapsuleComponent());
	powerAura->SetupAttachment(GetCapsuleComponent());
	arrow->SetUsingAbsoluteRotation(true);

	aimWidget->SetupAttachment(arrow);

	AddOwnedComponent(shoot);
	AddOwnedComponent(item);
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetCharacterMovement()->SetIsReplicated(true);
	GetCharacterMovement()->bServerAcceptClientAuthoritativePosition = false;
	if (dashIndicatorWidget&& barMunition&& aimWidget)
	{
		dashIndicatorWidget->SetWidgetSpace(EWidgetSpace::Screen);
		barMunition->SetWidgetSpace(EWidgetSpace::Screen);
		aimWidget->SetWidgetSpace(EWidgetSpace::World);
		aimWidget->SetTwoSided(true);
		aimWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		barMunition->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		dashIndicatorWidget->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		aimWidget->GetUserWidgetObject()->SetIsFocusable(false);
		barMunition->GetUserWidgetObject()->SetIsFocusable(false);
		dashIndicatorWidget->GetUserWidgetObject()->SetIsFocusable(false);
		dashIndicatorWidget->GetUserWidgetObject()->SetIsFocusable(false);
		itemWidget->GetUserWidgetObject()->SetIsFocusable(false);
		barDelayWidget->GetUserWidgetObject()->SetIsFocusable(false);
		aimWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
		barMunition->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
		dashIndicatorWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
		dashIndicatorWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
		itemWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
		barDelayWidget->GetUserWidgetObject()->SetVisibility(ESlateVisibility::HitTestInvisible);
		//aimWidget->SetWidgetSpace(EWidgetSpace::Screen);
		//barMunition->SetDrawSize(FVector2D(1.f, 1.f));
		
	}
	if (itemWidget&&item)
	{
		item->SetItemWidget(itemWidget);
		item->SetBarDelayWidget(barDelayWidget);
		itemWidget->SetVisibility(false);
	}
	 
	dashIndicatorWidget->SetVisibility(IsLocallyControlled() ? true : false);
	barMunition->SetVisibility(IsLocallyControlled() ? true : false);
	aimWidget->SetVisibility(IsLocallyControlled() ? true : false);
	if (!IsLocallyControlled())
	{
		itemWidget->SetRelativeLocation(FVector(0, 0, 175));
		barDelayWidget->SetRelativeLocation(FVector(0, 0, 175));

	}
	if (GetNetMode() == NM_DedicatedServer || GetNetMode() == NM_ListenServer)
	{
		AOnlineGameMode* _gm = GetWorld()->GetAuthGameMode<AOnlineGameMode>();
		if(_gm)
			_gm->AddPlayer(this);

	}
	/*if (UUserWidget* _widget = barMunition->GetUserWidgetObject())
	{
		_widget->SetDesiredSizeInViewport(FVector2D(1, 1));
	}*/
}



// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdateAimDirection();
	/*if (IsLocallyControlled())
	{
		UKismetSystemLibrary::PrintString(this, FString::FromInt(idController));
	}*/


	

}

void APlayerCharacter::UpdateAimDirection()
{
	APlayerController* _pc = Cast<APlayerController>(GetController());
	if (!_pc || !arrow||!IsLocallyControlled())return;

	float _mouseX, _mouseY;
	if (!_pc->GetMousePosition(_mouseX, _mouseY))return;

	
	FVector2D _screePos;
	if (!_pc->ProjectWorldLocationToScreen(GetActorLocation(), _screePos))return;
	FVector2D _direction = FVector2D(_mouseX, _mouseY) - _screePos;
	FRotator _newRotation;
	FRotator _aimRotation;

	if (FMath::Abs(_direction.X) >= FMath::Abs(_direction.Y))
	{
		_newRotation = _direction.X > 0.f ? FRotator(0.f, 0, -90.f) : FRotator(0.f, 0, 90.f);
		_aimRotation = _direction.X > 0.f ? FRotator(0, 90, 0) : FRotator(0.f, -90, 0.f);

		//UKismetSystemLibrary::PrintString(this, _direction.X > 0.f ? "Gauche" : "Droite");
	}
	else
	{
		_newRotation = _direction.Y < 0.f ? FRotator(0, 0, 180) : FRotator(0, 0, 0);
		_aimRotation = _direction.Y < 0.f ? FRotator(90, 0, 90) : FRotator(-90.f, 0, -90.f);
		//_aimRotation = _direction. > 0.f ? FRotator(90, 0, 90) : FRotator(-90.f, 0, -90.f);

		//UKismetSystemLibrary::PrintString(this, _direction.Y < 0.f  ? "Haut" : "Bas");

	}
	arrow->SetWorldRotation(_newRotation);
	shoot->SetDirection(_aimRotation);
}

void APlayerCharacter::InitInput()
{

	APlayerController* PC = Cast<APlayerController>(GetController());
	if (!PC) return;

	ULocalPlayer* _local = PC->GetLocalPlayer();
	if (!_local) return;

	UEnhancedInputLocalPlayerSubsystem* _input = _local->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (_input)
	{
		_input->AddMappingContext(inputs->inputs, 0);
	}
	/*UCommonInputSubsystem* _commonInput = _local->GetSubsystem<UCommonInputSubsystem>();
	if (_commonInput)
		_commonInput->OnInputMethodChanged.AddDynamic(this, &APlayerCharacter::OnInputDeviceChanged);*/
}

void APlayerCharacter::OnRep_Controller()
{
	Super::OnRep_Controller();
	InitInput();
	dashIndicatorWidget->SetVisibility(IsLocallyControlled() ? true : false);

}

void APlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	InitInput();
	dashIndicatorWidget->SetVisibility(IsLocallyControlled() ? true :false);
}

void APlayerCharacter::OnInputDeviceChanged(ECommonInputType InputType)
{
	bUseGamepad = (InputType == ECommonInputType::Gamepad);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* _inputs = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (_inputs)
	{
		_inputs->BindAction(inputs->move, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		_inputs->BindAction(inputs->jump, ETriggerEvent::Started, this, &APlayerCharacter::InputJump);
		_inputs->BindAction(inputs->jump, ETriggerEvent::Completed, this, &APlayerCharacter::InputStopJump);
		_inputs->BindAction(inputs->dash, ETriggerEvent::Started, this, &APlayerCharacter::Dash);
		_inputs->BindAction(inputs->shoot, ETriggerEvent::Started, this, &APlayerCharacter::Shoot);
		_inputs->BindAction(inputs->useItem, ETriggerEvent::Triggered, this, &APlayerCharacter::UseItem);
	}
}



void APlayerCharacter::Move(const FInputActionValue& _value)
{
	FVector2D _dir = _value.Get<FVector2D>();

	movement->Move(_dir.X);
}

void APlayerCharacter::InputJump(const FInputActionValue& _value)
{
	movement->Jump();
}

void APlayerCharacter::InputStopJump(const FInputActionValue& _value)
{
	StopJumping();
}

void APlayerCharacter::Dash(const FInputActionValue& _value)
{
	movement->Dash();
	CheckDash();
}

void APlayerCharacter::Shoot(const FInputActionValue& _value)
{
	shoot->Shoot();
}

void APlayerCharacter::UseItem(const FInputActionValue& _value)
{
	UKismetSystemLibrary::PrintString(this, "Click");
	item->UseItem();
}





void APlayerCharacter::Multicast_ChangeColor_Implementation(APlayerCharacter* _player,int _idController)
{
	_player->idController =  _idController;
	USkeletalMeshComponent* _mesh = _player->GetMesh();
	UMaterialInstance* _material1 = _player->materialCharacter[idController];
	if (_material1&& _mesh)
	{
		_mesh->SetMaterial(0, _material1);
	}
	UMaterialInstance* _material2 = _player->materialCharacter2[idController];
	if (_material2 && _mesh)
	{
		_mesh->SetMaterial(1, _material2);
	}
	UMaterialInstance* _overlay = _player->materialCharacter[idController];
	if (_material1&& _mesh)
	{
		_mesh->SetOverlayMaterial(_overlay);
	}

}

void APlayerCharacter::CheckDash()
{
	if (IsLocallyControlled()&& dashIndicatorWidget)
	{

		if (movement->IsDashing())
		{
			if (UUserWidget* _widget = dashIndicatorWidget->GetUserWidgetObject())
			{
				if (UImage* _image = Cast<UImage>(_widget->GetWidgetFromName(TEXT("DashImage"))))
				{
					_image->SetRenderOpacity(0.1);
				}
			}
		}
		else
		{
			
			if (UUserWidget* _widget = dashIndicatorWidget->GetUserWidgetObject())
			{
				if (UImage* _image = Cast<UImage>(_widget->GetWidgetFromName(TEXT("DashImage"))))
				{
					_image->SetRenderOpacity(1);
				}
			}
		}
	}
}

void APlayerCharacter::Death()
{
	UKismetSystemLibrary::PrintString(this, "Mort");

	if (!HasAuthority())return;
	Server_Death(this);
}

void APlayerCharacter::Multicast_ReplicateHideWidget_Implementation(APlayerCharacter* _player,  bool _enable)
{
	if (!IsLocallyControlled()) return;

	APlayerController* _pc = Cast<APlayerController>(_player->GetController());
	if (!_pc) return;

	AGameHUD* _hud = Cast<AGameHUD>(_pc->GetHUD());
	if (!_hud) return;

	_hud->EndCountDown();
}

void APlayerCharacter::Multicast_ReplicateEnableInput_Implementation(APlayerCharacter* _player, bool _enable)
{
	if (!_player->IsLocallyControlled()) return;

	APlayerController* _pc = Cast<APlayerController>(_player->GetController());
	if (!_pc) return;

	if (_enable)
	{
		_player->EnableInput(_pc);
		/*FInputModeGameOnly _inputMode;
		_pc->SetInputMode(_inputMode);*/

		//UKismetSystemLibrary::PrintString(this, "EnableInput");
	}
	else
	{
		_player->DisableInput(_pc);
		//UKismetSystemLibrary::PrintString(this, "DisableInput");
	}
}
void APlayerCharacter::Multicast_ReplicateUpdateCount_Implementation(APlayerCharacter* _player, int _text)
{
	if (!IsLocallyControlled()) return;

	APlayerController* _pc = Cast<APlayerController>(_player->GetController());
	if (!_pc) return;

	AGameHUD* _hud = Cast<AGameHUD>(_pc->GetHUD());
	if (!_hud) return;

	_hud->UpdateCountdownTest(_text);


}

void APlayerCharacter::Server_Death_Implementation(APlayerCharacter* _player)
{
	/*UKismetSystemLibrary::PrintString(this, "Mort");

	movement->DisableMovement();
	dashIndicatorWidget->SetVisibility(false);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	isDead = true;
	if (IsLocallyControlled())
	{
		DisableInput(Cast<APlayerController>(GetController()));
		UKismetSystemLibrary::PrintString(this, "Input Disable");

	}*/
	_player->Multicast_Death(_player);
	
	AOnlineGameMode* _gm =Cast<AOnlineGameMode>(GetWorld()->GetAuthGameMode());
	if (_gm)
	{
		_gm->RemovePlayer(_player);
		UKismetSystemLibrary::PrintString(this, "Remove");

	}


}

void APlayerCharacter::Multicast_Death_Implementation(APlayerCharacter* _player)
{
	UKismetSystemLibrary::PrintString(this, "Mort");

	_player->movement->DisableMovement();
	_player->dashIndicatorWidget->SetVisibility(false);
	_player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_player->GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	_player->GetMesh()->SetVisibility(false);
	_player->isDead = true;
	if (_player->IsLocallyControlled())
	{
		_player->DisableInput(Cast<APlayerController>(GetController()));
		_player->dashIndicatorWidget->SetVisibility(false);
		_player->barMunition->SetVisibility(false);
		_player->aimWidget->SetVisibility(false);
		_player->itemWidget->SetVisibility(false);
		_player->barDelayWidget->SetVisibility(false);
		UKismetSystemLibrary::PrintString(this, "Input Disable");

	}

}

void APlayerCharacter::Multicast_ReplicateCheckDash_Implementation(APlayerCharacter* _player, bool _isDashing)
{
	if (IsLocallyControlled())return;
	if (_isDashing)
	{
		if (UUserWidget* _widget = dashIndicatorWidget->GetUserWidgetObject())
		{
			if (UImage* _image = Cast<UImage>(_widget->GetWidgetFromName(TEXT("DashImage"))))
			{
				_image->SetRenderOpacity(1);
			}
		}
	}
	else
	{
		if (UUserWidget* _widget = dashIndicatorWidget->GetUserWidgetObject())
		{
			if (UImage* _image = Cast<UImage>(_widget->GetWidgetFromName(TEXT("DashImage"))))
			{
				_image->SetRenderOpacity(1);
			}
		}
	}

}

void APlayerCharacter::Server_ReplicateCheckDash_Implementation(APlayerCharacter* _player, bool _isDashing)
{
	Multicast_ReplicateCheckDash(_player, _isDashing);
}

void APlayerCharacter::Multicast_ReplicateShowScore_Implementation(APlayerCharacter* _player)
{
	if (!IsLocallyControlled())return;
	
	APlayerController* _pc = Cast<APlayerController>(GetController());
	if (!_pc) return;

	AGameHUD* _hud = Cast<AGameHUD>(_pc->GetHUD());

	_hud->ShowScore();
}
void APlayerCharacter::Multicast_ReplicateStartPlatform_Implementation(APlayerCharacter* _player)
{
	if (!IsLocallyControlled())
	{
		return;
	}
	
	TArray<AActor*> _actors;
	UGameplayStatics::GetAllActorsOfClass(
		GetWorld(),
		AMovablePlatform::StaticClass(),
		_actors
	);

	for (AActor* _actor : _actors)
	{
		AMovablePlatform* _platform = Cast<AMovablePlatform>(_actor);
		if (_platform)
		{
			_platform->StartMovement();
		}
	}
}



