// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

class ULobbyWidget;
class UMapSelectorCell;
class APlayerController;
class ALobbyPlayerController;
class UMultiplayerSessionSubsystem;
class UPlayerSettings;

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

	UPROPERTY()
	UPlayerSettings* PlayerSettings;

	FLobbyGameInfo GameInfo;

	bool bIsLaunchCountdownEnabled{ false };

	FString MapSelected{ TEXT("") };

public:
	void PlayerRequestUpdate();
	void PlayerRequestMapUpdate();
	void PlayerHostSelectedMap(const FString& MapName);
	bool IsAllPlayerReady() const;
	void StartGameFromLobby();
	void ExitGameSession();

protected:
	virtual void BeginPlay() override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual void Logout(AController* ExitingPlayer) override;

private:
	void ConfigurePlayerSettings();
	void ConfigureOnlineSubsystem();
	void UpdatePlayerList();

	UFUNCTION()
	void DestroySessionComplete(bool bWasSuccessful);

	UFUNCTION()
	void StartSessionComplete(bool bWasSuccessful);
};
