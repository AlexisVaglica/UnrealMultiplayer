// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Lobby/LobbyGameMode.h"
#include "BoxGame/Lobby/LobbyPlayerController.h"
#include "BoxGame/DataAssets/LocalPlayerDataAsset.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "GameFramework/GameState.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::BeginPlay()
{
	ConfigureOnlineSubsystem();
}

void ALobbyGameMode::ConfigureOnlineSubsystem()
{
	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance)
	{
		MultiplayerSession = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();

		if (MultiplayerSession)
		{
			MultiplayerSession->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::DestroySessionComplete);
			MultiplayerSession->MultiplayerOnStartSessionComplete.AddDynamic(this, &ThisClass::StartSessionComplete);

			GameInfo.MaxNumberOfPlayerInMatch = MultiplayerSession->GetMaxPlayersInSession(GameSessionName);
			GameInfo.MinNumberOfPlayerForStartMatch = MinNumberOfPlayerForStartMatch;
		}
	}
}

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	ALobbyPlayerController* JoiningPlayer = Cast<ALobbyPlayerController>(NewPlayer);
	APlayerState* JoiningPlayerState = JoiningPlayer->GetPlayerState<APlayerState>();

	if (JoiningPlayerState && LocalPlayerData)
	{
		FString PlayerInfoName = LocalPlayerData->LocalPlayerInfo.LocalName;
		JoiningPlayerState->SetPlayerName(PlayerInfoName);
	}

	if (JoiningPlayer)
	{
		ConnectedPlayers.Add(JoiningPlayer);
	}
}

void ALobbyGameMode::Logout(AController* ExitingPlayer)
{
	Super::Logout(ExitingPlayer);

	ALobbyPlayerController* LobbyPlayerController = Cast<ALobbyPlayerController>(ExitingPlayer);

	if (LobbyPlayerController)
	{
		ConnectedPlayers.Remove(LobbyPlayerController);
		UpdatePlayerList();
	}
}

void ALobbyGameMode::UpdatePlayerList()
{
	PlayerInfoArray.Empty();

	for (ALobbyPlayerController* Player : ConnectedPlayers)
	{
		FLobbyPlayerInfo TempLobbyPlayerInfo;
		TempLobbyPlayerInfo.bPlayerReadyState = Player->IsPlayerReady();
		TempLobbyPlayerInfo.PlayerName = Player->GetPlayerState<APlayerState>()->GetPlayerName();
		PlayerInfoArray.Add(TempLobbyPlayerInfo);
	}

	for (ALobbyPlayerController* Player : ConnectedPlayers) 
	{
		Player->ClientUpdatePlayerList(PlayerInfoArray, GameInfo);
	}
}

void ALobbyGameMode::StartGameFromLobby()
{
	for (auto Player : ConnectedPlayers) 
	{
		Player->ClientChangeWidgetToLaunch();
	}

	if (MultiplayerSession)
	{
		MultiplayerSession->StartSession();
	}
}

void ALobbyGameMode::ExitGameSession()
{
	if (MultiplayerSession)
	{
		MultiplayerSession->DestroySession();
	}
}

bool ALobbyGameMode::IsAllPlayerReady() const
{
	for (ALobbyPlayerController* Player : ConnectedPlayers)
	{
		if (!Player->IsPlayerReady() || ConnectedPlayers.Num() < GameInfo.MinNumberOfPlayerForStartMatch)
		{
			return false;
		}
	}
	return true;
}

void ALobbyGameMode::PlayerRequestUpdate()
{
	UpdatePlayerList();
}

void ALobbyGameMode::PlayerRequestMapUpdate()
{
	if (MapSelected.IsEmpty()) 
	{
		return;
	}

	for (ALobbyPlayerController* Player : ConnectedPlayers)
	{
		Player->ClientChangeMapSelected(MapSelected);
	}
}

void ALobbyGameMode::DestroySessionComplete(bool bWasSuccessful)
{
	ReturnToMainMenuHost();
}

void ALobbyGameMode::StartSessionComplete(bool bWasSuccessful)
{
	GetWorld()->ServerTravel(MapSelected);
}

void ALobbyGameMode::PlayerHostSelectedMap(const FString& MapName) 
{
	MapSelected = MapName;

	PlayerRequestMapUpdate();
}