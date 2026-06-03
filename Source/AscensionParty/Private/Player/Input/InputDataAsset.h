// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

/**
 * 
 */
class UInputMappingContext;
class UInputAction;
UCLASS()
class UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "MappingContext")TObjectPtr<UInputMappingContext>inputs;
	UPROPERTY(EditAnywhere, Category = "InputAction")TObjectPtr<UInputAction>move;
	UPROPERTY(EditAnywhere, Category = "InputAction")TObjectPtr<UInputAction>jump;
	UPROPERTY(EditAnywhere, Category = "InputAction")TObjectPtr<UInputAction>dash;
	UPROPERTY(EditAnywhere, Category = "InputAction")TObjectPtr<UInputAction>shoot;
	UPROPERTY(EditAnywhere, Category = "InputAction")TObjectPtr<UInputAction>useItem;
	//UPROPERTY(EditAnywhere, Category = "InputAction")TObjectPtr<UInputAction>mouseLook;

	
};
