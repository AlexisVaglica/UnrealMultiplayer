// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Lobby/LobbyPlayerController.h"
#include "BoxGame/Lobby/LobbyGameMode.h"
#include "BoxGame/Lobby/LobbyHUD.h"
#include "GameFramework/GameMode.h"
#include "Net/UnrealNetwork.h"
#include "Kismet/GameplayStatics.h"

void ALobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	bIsReady = false;

	FTimerHandle RequestServerPlayerListUpdateHanndle;
	GetWorld()->GetTimerManager().SetTimer(RequestServerPlayerListUpdateHanndle, this, &ALobbyPlayerController::RequestServerPlayerListUpdate, 0.5f, false);

	LobbyHUD = Cast<ALobbyHUD>(GetHUD());

	if (LobbyHUD)
	{
		if (IsPlayerHost())
		{
			LobbyHUD->OnLaunchGameSelected.BindUObject(this, &ThisClass::LaunchGame);
			LobbyHUD->OnMapWasSelected.BindUObject(this, &ThisClass::RequestServerReplicateMapSelected);
		}

		LobbyHUD->OnReadyButtonPressed.BindUObject(this, &ThisClass::PlayerChangeReady);
		LobbyHUD->OnCancelButtonPressed.BindUObject(this, &ThisClass::CancelGame);
	}

	ServerRequestMapUpdate();
}

bool ALobbyPlayerController::IsPlayerHost() const
{
	ENetMode NetMode = GetNetMode();
	return NetMode == ENetMode::NM_ListenServer || NetMode == ENetMode::NM_DedicatedServer;
}

void ALobbyPlayerController::SetIsReadyState(bool NewReadyState)
{
	if (IsPlayerHost())
	{
		bIsReady = NewReadyState;
		RequestServerPlayerListUpdate();
	}
	else
	{
		ServerSetIsReadyState(NewReadyState);
	}
}

void ALobbyPlayerController::PlayerChangeReady()
{
	SetIsReadyState(!bIsReady);
}

bool ALobbyPlayerController::CanGameStart() const
{
	if (IsPlayerHost())
	{
		ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			return GM->IsAllPlayerReady();
		}
	}
	return false;
}

void ALobbyPlayerController::LaunchGame()
{
	if (IsPlayerHost())
	{
		ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (GM)
		{
			GM->StartGameFromLobby();
		}
	}
}

void ALobbyPlayerController::CancelGame()
{
	if (IsPlayerHost()) 
	{
		ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->ExitGameSession();
		}
	}
	else
	{
		ClientReturnToMainMenuWithTextReason(FText());
	}
}

void ALobbyPlayerController::RequestServerReplicateMapSelected(FString MapName)
{
	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->PlayerHostSelectedMap(MapName);

		if (LobbyHUD) 
		{
			LobbyHUD->PlayerCanLaunchGame(CanGameStart());
		}
	}
}

void ALobbyPlayerController::RequestServerPlayerListUpdate()
{
	if (IsPlayerHost())
	{
		ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());

		if (GM)
		{
			GM->PlayerRequestUpdate();
		}
	}
	else
	{
		ServerRequestPlayerListUpdate();
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyPlayerController, bIsReady);
}

void ALobbyPlayerController::ServerRequestPlayerListUpdate_Implementation()
{
	RequestServerPlayerListUpdate();
}

void ALobbyPlayerController::ServerRequestMapUpdate_Implementation()
{
	ALobbyGameMode* GM = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());

	if (GM)
	{
		GM->PlayerRequestMapUpdate();
	}
}

void ALobbyPlayerController::ServerSetIsReadyState_Implementation(bool NewReadyState)
{
	SetIsReadyState(NewReadyState);
}

void ALobbyPlayerController::ClientUpdatePlayerList_Implementation(const TArray<FLobbyPlayerInfo>& PlayerInfoArray, const struct FLobbyGameInfo& GameInfo)
{
	if (LobbyHUD)
	{
		if (IsPlayerHost())
		{
			LobbyHUD->PlayerCanLaunchGame(CanGameStart());
		}

		LobbyHUD->UpdatePlayerList(PlayerInfoArray, GameInfo);
	}
}

void ALobbyPlayerController::ClientChangeWidgetToLaunch_Implementation()
{
	if (LobbyHUD)
	{
		LobbyHUD->ChangeWidgetToLaunch();
	}
}

void ALobbyPlayerController::ClientChangeMapSelected_Implementation(const FString& MapName)
{
	if (LobbyHUD)
	{
		LobbyHUD->ChangeMapSelected(MapName);
	}
}