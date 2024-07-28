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
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category="Login")
	void LoginEOS();

	UFUNCTION(BlueprintPure, Category="Login")
	bool IsLoggedIn() const;

	UFUNCTION(BlueprintPure, Category="User")
	FString GetDisplayName() const;
	
	// only this class and derived/child classes can access this function
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Login")
	void OnLoginComplete(bool success, const FString& error);
	
	// only this class can access these private functions
private:
	// runs when eos login complete
	void EOSLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);
};
