// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MGGameInstance.generated.h"

#define MGSESSION_NAME FName(TEXT("MGSESSION"))

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API UMGGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	// allows other classes to use that function
public:
	UMGGameInstance();
	
	virtual void Init() override;
	
	UFUNCTION(BlueprintCallable, Category="Login")
	void LoginEOS();

	UFUNCTION(BlueprintPure, Category="Login")
	bool IsLoggedIn() const;

	UFUNCTION(BlueprintPure, Category="User")
	FString GetDisplayName() const;

	UFUNCTION(BlueprintPure, Category="Session")
	bool IsInSession() const;

	// host a game using EOS - currently online LAN capable - THANKS EPIC!!!
	UFUNCTION(BlueprintCallable,Category="Session")
	void HostGame(bool lan);

	// find a hosted game and if found join the session
	UFUNCTION(BlueprintCallable, Category="Session")
	void FindAndJoinSession();
	
	// only this class and derived/child classes can access this function
protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Login")
	void OnLoginComplete(bool success, const FString& error);

	UFUNCTION(BlueprintImplementableEvent, Category="Session") 
	void OnSessionCreateComplete(bool success);

	UFUNCTION(BlueprintImplementableEvent, Category="Session")
	void OnSessionJoinComplete(bool success);
	
	// only this class can access these private functions
private:
	// runs when eos login complete
	void EOSLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId, const FString& Error);

	// run when the session create function completes
	void SessionCreateComplete(FName SessionName, bool bWasSuccessful);

	// runs when a find session function completes
	void SessionFindComplete(bool bWasSuccessful);

	// runs on session join complete
	void SessionJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	
	// when find session is run, this will fill with found sessions
	TSharedPtr<FOnlineSessionSearch> FoundSessions;
};
