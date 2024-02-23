// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MultiplayerSessionSubsystem.generated.h"

USTRUCT()
struct FSessionGameInfo
{
	GENERATED_USTRUCT_BODY()

	FString SessionId;
	FString OwnerId;
	FString OwnerName;
	int32 CurrentPlayersCount;
	int32 MaxPlayersCount;
};


/*
* Declaring custom delegates for callbacks
*/
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionComplete, bool, bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_TwoParams(FMultiplayerOnFindSessionComplete, const TArray<struct FSessionGameInfo>& SessionIdResults, bool bWasSuccessful);
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiplayerOnJoinSessionComplete, bool bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnDestroySessionComplete, bool, bWasSuccessful);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnStartSessionComplete, bool, bWasSuccessful);

class UMultiplayerDataAsset;
/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

private:
	IOnlineSessionPtr SessionInterface;
	EName GameSessionName;
	TSharedPtr<FOnlineSessionSettings> SessionSettings;
	TSharedPtr<FOnlineSessionSearch> LastSessionSearch;

	bool bCreateSessionOnDestroy{ false };
	UMultiplayerDataAsset* LastDataAsset;

public:
	UMultiplayerSessionSubsystem();

	/*
	* To Handle Session Functionality. Externals classes will call these
	*/
	void CreateSession(UMultiplayerDataAsset* DataAsset);
	void FindSessions(int32 MaxSearchResult);
	void JoinSession(const FString& SessionResultId);
	void DestroySession();
	void StartSession();

	int32 GetMaxPlayersInSession(FName SessionName);

	FString GetResolvedConnectString();

	/*
	* Custom Delegate for Callbacks
	*/
	FMultiplayerOnCreateSessionComplete MultiplayerOnCreateSessionComplete;
	FMultiplayerOnFindSessionComplete MultiplayerOnFindSessionComplete;
	FMultiplayerOnJoinSessionComplete MultiplayerOnJoinSessionComplete;
	FMultiplayerOnDestroySessionComplete MultiplayerOnDestroySessionComplete;
	FMultiplayerOnStartSessionComplete MultiplayerOnStartSessionComplete;

protected:
	/*
	* We'll add these callbacks to the delegates  
	*/
	void CreateSessionComplete(FName SessionName, bool bWasSuccess);
	void FindSessionComplete(bool bWasSuccess);
	void JoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void DestroySessionComplete(FName SessionName, bool bWasSuccess);
	void StartSessionComplete(FName SessionName, bool bWasSuccess);

private:

	void ConfigureSessionSettings(class UMultiplayerDataAsset* DataAsset);

	/* 
	* We'll bind our MultiplayerSessionSubsystem internal callbacks to these
	*/
	FOnCreateSessionCompleteDelegate CreateSessionCompleteDelegate;
	FOnFindSessionsCompleteDelegate FindSessionCompleteDelegate;
	FOnJoinSessionCompleteDelegate JoinSessionCompleteDelegate;
	FOnDestroySessionCompleteDelegate DestroySessionCompleteDelegate;
	FOnStartSessionCompleteDelegate StartSessionCompleteDelegate;

	/*
	* We'll store the Delegates in these Handles to be able to destroy them. 
	*/
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FDelegateHandle FindSessionCompleteDelegateHandle;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
