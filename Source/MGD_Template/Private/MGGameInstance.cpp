// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"

void UMGGameInstance::Init()
{
	Super::Init();

	// this is the online subsystem
	// this gives us access to all of the online subsystem function
	const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

	// if the ossRef returns null
	// exit the function
	if (!ossRef)
		return;

	// this is the identity interface for EOS
	// we can get info about the players account from this interface
	// it also allows us to access authentication(Login / Logout)
	const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

	// if the identityRef returns null
	// exit the function
	if (!identityRef)
		return;

	// binding the login complete function to login complete in the identity interface
	identityRef->OnLoginCompleteDelegates->AddUObject(this, &UMGGameInstance::EOSLoginComplete);
}

void UMGGameInstance::LoginEOS()
{
	// this is the online subsystem
	// this gives us access to all of the online subsystem function
	const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

	// if the ossRef returns null
	// exit the function
	if (!ossRef)
		return;

	// this is the identity interface for EOS
	// we can get info about the players account from this interface
	// it also allows us to access authentication(Login / Logout)
	const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

	// if the identityRef returns null
	// exit the function
	if (!identityRef)
		return;

	// this sets up the type of account credentials that the login will expect
	// the account portal is a login portal that opens when you try to login
	FOnlineAccountCredentials accCreds;
	accCreds.Id = FString();
	accCreds.Token = FString();
	accCreds.Type = FString(TEXT("accountportal"));

	// run the login portal (technically the main function we really need to run)
	identityRef->Login(0, accCreds);
}

bool UMGGameInstance::IsLoggedIn() const
{
	// this is the online subsystem
	// this gives us access to all of the online subsystem function
	const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

	// if the ossRef returns null
	// exit the function
	if (!ossRef)
		return false;

	// this is the identity interface for EOS
	// we can get info about the players account from this interface
	// it also allows us to access authentication(Login / Logout)
	const IOnlineIdentityPtr identityRef = ossRef->GetIdentityInterface();

	// if the identityRef returns null
	// exit the function
	if (!identityRef)
		return false;

	return identityRef->GetLoginStatus(0) == ELoginStatus::LoggedIn;
}

FString UMGGameInstance::GetDisplayName() const
{
	if (!IsLoggedIn())
		return FString(TEXT("Display Name"));

	// since is logged in already check that the identity interface exists we can skip a step straight to the identity
	const IOnlineIdentityPtr identityRef = Online::GetSubsystem(GetWorld())->GetIdentityInterface();

	return identityRef->GetPlayerNickname(0);
}

void UMGGameInstance::EOSLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
                                       const FString& Error)
{
	OnLoginComplete(bWasSuccessful, Error);
}
