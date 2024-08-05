// Fill out your copyright notice in the Description page of Project Settings.


#include "MGGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "GameFramework/GameModeBase.h"
#include "Online/OnlineSessionNames.h"

UMGGameInstance::UMGGameInstance()
{
	FoundSessions = MakeShareable(new FOnlineSessionSearch());
	FoundSessions->MaxSearchResults = 10;
	FoundSessions->bIsLanQuery = true;
	FoundSessions->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);
}

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

	const IOnlineSessionPtr sessionRef = ossRef->GetSessionInterface();

	if (!sessionRef)
		return;

	// binding our SessionCreateComplete function to the CreateSession complete delegate
	// when a session is created (success or fail) this function will run
	sessionRef->OnCreateSessionCompleteDelegates.AddUObject(this, &UMGGameInstance::SessionCreateComplete);

	// when a find session function has been called wait for it to complete and run the bound function
	sessionRef->OnFindSessionsCompleteDelegates.AddUObject(this, &UMGGameInstance::SessionFindComplete);

	// run when join session has complete
	sessionRef->OnJoinSessionCompleteDelegates.AddUObject(this, &UMGGameInstance::SessionJoinComplete);
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

bool UMGGameInstance::IsInSession() const
{
	// are we logged in
	// this checks if there is an online subsystem so we can skip that check
	if (!IsLoggedIn())
		return false;

	// get the interface that interacts with sessions so we can get session data
	const IOnlineSessionPtr sessionRef = Online::GetSubsystem(GetWorld())->GetSessionInterface();

	// make sure there is a session interface
	if (!sessionRef)
		return false;

	// get locally store the session state
	const EOnlineSessionState::Type state = sessionRef->GetSessionState(MGSESSION_NAME);
	
	// if there is a session return true
	return state != EOnlineSessionState::NoSession;
}

void UMGGameInstance::HostGame(bool lan)
 {
	// are we logged in
	if (!IsLoggedIn())
		return;

	// get the session interface
	const IOnlineSessionPtr sessionRef = Online::GetSubsystem(GetWorld())->GetSessionInterface();

	if (!sessionRef)
		return;

	// define the setting of the session
	FOnlineSessionSettings settings;
	settings.NumPublicConnections = 4;
	settings.bIsLANMatch = lan;
	settings.bIsDedicated = false;
	settings.bAllowInvites = true;
	settings.bShouldAdvertise = true;
	settings.bUsesPresence = true;
	settings.bUseLobbiesIfAvailable = true;
	settings.Set(SEARCH_KEYWORDS, MGSESSION_NAME.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);
	settings.Set(SEARCH_LOBBIES, true, EOnlineDataAdvertisementType::ViaOnlineService);

	//create the session using the settings and name we created 
	sessionRef->CreateSession(0, MGSESSION_NAME, settings);
}

void UMGGameInstance::FindAndJoinSession()
{
	// make sure we are logged in
	if (!IsLoggedIn())
		return;

	const IOnlineSessionPtr sessionRef = Online::GetSubsystem(GetWorld())->GetSessionInterface();

	if (!sessionRef)
		return;

	// finding a session based on found session query settings
	sessionRef->FindSessions(0,FoundSessions.ToSharedRef());
}

void UMGGameInstance::StartLobbyGame()
{
	GetWorld()->GetAuthGameMode()->bUseSeamlessTravel = true;
	GetWorld()->ServerTravel("/Game/MyContent/Maps/Lvl_Test", false);
}

void UMGGameInstance::EOSLoginComplete(int32 LocalUserNum, bool bWasSuccessful, const FUniqueNetId& UserId,
                                       const FString& Error)
{
	OnLoginComplete(bWasSuccessful, Error);
}

void UMGGameInstance::SessionCreateComplete(FName SessionName, bool bWasSuccessful)
{
	EnableListenServer(true);
	OnSessionCreateComplete(bWasSuccessful);
}

void UMGGameInstance::SessionFindComplete(bool bWasSuccessful)
{
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to find session"))
		OnSessionJoinComplete(false);
		return;
	}

	const IOnlineSubsystem* ossRef = Online::GetSubsystem(GetWorld());

	if (!ossRef)
		return;
	
	const IOnlineSessionPtr sessionRef = ossRef->GetSessionInterface();

	if (!sessionRef)
		return;

	// making sure we found sessions
	if (FoundSessions->SearchResults.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("No sessions found"))
		OnSessionJoinComplete(false);
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Found session attempting to join"))
	
	// join the session that is the first session found
	sessionRef->JoinSession(0, MGSESSION_NAME, FoundSessions->SearchResults[0]);
}

void UMGGameInstance::SessionJoinComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	OnSessionJoinComplete(Result == EOnJoinSessionCompleteResult::Success);
	
	if (Result != EOnJoinSessionCompleteResult::Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to join session"))
		return;
	}

	// automatically get the ip address and remote location and travel the client to the game
	ClientTravelToSession(0, SessionName);
}
