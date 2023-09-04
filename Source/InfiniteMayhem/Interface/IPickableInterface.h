// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IPickableInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIPickableInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class INFINITEMAYHEM_API IIPickableInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	void EnableOutlineDisplay(); // 开启描边

	UFUNCTION(BlueprintNativeEvent)
	void DisableOutlineDisplay(); // 关闭描边

	UFUNCTION(BlueprintNativeEvent)
	void Pickup(AActor* _Owner); // 拾取 


};
