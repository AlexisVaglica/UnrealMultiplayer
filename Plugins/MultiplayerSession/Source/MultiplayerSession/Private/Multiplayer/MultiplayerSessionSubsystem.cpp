// Fill out your copyright notice in the Description page of Project Settings.


#include "Multiplayer/MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"
#include "Multiplayer/GameData/MultiplayerDataAsset.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::CreateSessionComplete)),
	FindSessionCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::FindSessionComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::JoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::DestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::StartSessionComplete))
{
	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();

	if (Subsystem) 
	{
		SessionInterface = Subsystem->GetSessionInterface();
	}

	GameSessionName = NAME_GameSession;
}

void UMultiplayerSessionSubsystem::CreateSession(UMultiplayerDataAsset* DataAsset)
{
	if (!SessionInterface.IsValid() && DataAsset != nullptr) 
	{
		return;
	}

	FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(GameSessionName);

	if (ExistingSession != nullptr) 
	{
		bCreateSessionOnDestroy = true;
		LastDataAsset = DataAsset;

		DestroySession();

		return;
	}

	CreateSessionCompleteDelegateHandle = SessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	ConfigureSessionSettings(DataAsset);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	bool bWasSuccessfully = SessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), GameSessionName, *SessionSettings);

	if (!bWasSuccessfully) 
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		MultiplayerOnCreateSessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResult)
{
	if (!SessionInterface.IsValid()) 
	{
		return;
	}

	FindSessionCompleteDelegateHandle = SessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegate);

	FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	const FName NullName = "NULL";

	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResult;
	LastSessionSearch->bIsLanQuery = SubsystemName == NullName;
	LastSessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	bool bWasSuccess = SessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef());

	if (!bWasSuccess) 
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FString>(), false);
	}
}

void UMultiplayerSessionSubsystem::JoinSession(const FString& SessionResultId)
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnJoinSessionComplete.Broadcast(false);
		return;
	}

	JoinSessionCompleteDelegateHandle = SessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);

	ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();

	FOnlineSessionSearchResult SessionResult;

	for ( FOnlineSessionSearchResult PossibleSessionResult : LastSessionSearch->SearchResults) 
	{
		if (SessionResultId == PossibleSessionResult.GetSessionIdStr())
		{
			SessionResult = PossibleSessionResult;
		}
	}

	bool bWasSuccess = SessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), GameSessionName, SessionResult);

	if (!bWasSuccess) 
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		MultiplayerOnJoinSessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionSubsystem::DestroySession()
{
	if (!SessionInterface.IsValid())
	{
		MultiplayerOnDestroySessionComplete.Broadcast(false);
		return;
	}

	DestroySessionCompleteDelegateHandle = SessionInterface->AddOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegate);

	bool bWasSuccess = SessionInterface->DestroySession(GameSessionName);

	if (!bWasSuccess)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
		MultiplayerOnDestroySessionComplete.Broadcast(false);
	}
}

void UMultiplayerSessionSubsystem::StartSession()
{
	if (!SessionInterface.IsValid())
	{
		return;
	}

	StartSessionCompleteDelegateHandle = SessionInterface->AddOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegate);

	bool bWasSuccess = SessionInterface->StartSession(GameSessionName);

	if (!bWasSuccess)
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
		MultiplayerOnStartSessionComplete.Broadcast(bWasSuccess);
	}
}

int32 UMultiplayerSessionSubsystem::GetMaxPlayersInSession(FName SessionName)
{
	auto Session = SessionInterface->GetSessionSettings(SessionName);

	if (Session) 
	{
		return Session->NumPublicConnections;
	}

	return 0;
}

void UMultiplayerSessionSubsystem::CreateSessionComplete(FName SessionName, bool bWasSuccess)
{
	if (SessionInterface) 
	{
		SessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}

	MultiplayerOnCreateSessionComplete.Broadcast(bWasSuccess);
}

void UMultiplayerSessionSubsystem::FindSessionComplete(bool bWasSuccess)
{
	if (SessionInterface) 
	{
		SessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0) 
	{
		MultiplayerOnFindSessionComplete.Broadcast(TArray<FString>(), true);
		return;
	}

	TArray<FString> SearchIdResults;
	
	for (int i = 0; i < LastSessionSearch->SearchResults.Num(); i++)
	{
		FString SessionResultId = LastSessionSearch->SearchResults[i].GetSessionIdStr();
		SearchIdResults.Add(SessionResultId);
	}

	MultiplayerOnFindSessionComplete.Broadcast(SearchIdResults, bWasSuccess);
}

void UMultiplayerSessionSubsystem::JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (SessionInterface) 
	{
		SessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}

	if (Result == EOnJoinSessionCompleteResult::Success)
	{
		MultiplayerOnJoinSessionComplete.Broadcast(true);
	}
}

void UMultiplayerSessionSubsystem::DestroySessionComplete(FName SessionName, bool bWasSuccess)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnDestroySessionCompleteDelegate_Handle(DestroySessionCompleteDelegateHandle);
	}

	if (bWasSuccess && bCreateSessionOnDestroy) 
	{
		bCreateSessionOnDestroy = false;
		CreateSession(LastDataAsset);
	}

	MultiplayerOnDestroySessionComplete.Broadcast(bWasSuccess);
}

void UMultiplayerSessionSubsystem::StartSessionComplete(FName SessionName, bool bWasSuccess)
{
	if (SessionInterface)
	{
		SessionInterface->ClearOnStartSessionCompleteDelegate_Handle(StartSessionCompleteDelegateHandle);
	}

	MultiplayerOnStartSessionComplete.Broadcast(bWasSuccess);
}

FString UMultiplayerSessionSubsystem::GetResolvedConnectString()
{
	FString AddressResult;

	SessionInterface->GetResolvedConnectString(GameSessionName, AddressResult);

	return AddressResult;
}

void UMultiplayerSessionSubsystem::ConfigureSessionSettings(UMultiplayerDataAsset* DataAsset)
{
	FName SubsystemName = IOnlineSubsystem::Get()->GetSubsystemName();
	const FName NullName = "NULL";

	SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->NumPublicConnections = DataAsset->MaxPlayersCount;
	SessionSettings->NumPrivateConnections = DataAsset->MaxPlayersCount;
	SessionSettings->bIsLANMatch = SubsystemName == NullName;
	SessionSettings->bIsDedicated = false;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bUseLobbiesIfAvailable = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bUsesPresence = true;
	SessionSettings->bAllowInvites = true;
	SessionSettings->Set(FName("MatchType"), DataAsset->GameTypeName.ToString(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
}
