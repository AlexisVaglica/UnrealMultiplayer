// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

class ULobbyWidget;
class UMapDataAsset;
class UMapSelectorCell;
class APlayerController;
class ALobbyPlayerController;
class UMultiplayerSessionSubsystem;

USTRUCT()
struct FLobbyPlayerInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	bool bPlayerReadyState;

	UPROPERTY()
	FString PlayerName;
};

USTRUCT()
struct FLobbyGameInfo
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	int32 MinNumberOfPlayerForStartMatch;

	UPROPERTY()
	int32 MaxNumberOfPlayerInMatch;
};

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

private:
	TArray<struct FLobbyPlayerInfo> PlayerInfoArray;
	TArray<ALobbyPlayerController*> ConnectedPlayers;

	UPROPERTY()
	UMultiplayerSessionSubsystem* MultiplayerSession;

	UPROPERTY(EditAnywhere)
	int32 MinNumberOfPlayerForStartMatch{ 2 };

	FLobbyGameInfo GameInfo;

	bool bIsLaunchCountdownEnabled{ false };

	FString MapToTravel{ TEXT("") };

public:
	void PlayerRequestUpdate();
	bool IsAllPlayerReady() const;
	void StartGameFromLobby(FString MapName);
	void ExitGameSession();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

private:
	void ConfigureOnlineSubsystem();
	void UpdatePlayerList();

	UFUNCTION()
	void DestroySessionComplete(bool bWasSuccessful);

	UFUNCTION()
	void StartSessionComplete(bool bWasSuccessful);
};
