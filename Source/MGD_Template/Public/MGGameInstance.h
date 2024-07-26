// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	// allows other classes to use that function
public:
	UFUNCTION(BlueprintCallable, Category="Login")
	void LoginEOS();

	// only this class and derived/child classes can access this function
protected:
	
	// only this class can access these private functions
private:
	
};
