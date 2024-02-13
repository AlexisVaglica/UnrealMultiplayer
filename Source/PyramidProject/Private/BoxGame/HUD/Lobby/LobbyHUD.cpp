// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/Lobby/LobbyHUD.h"
#include "BoxGame/HUD/Lobby/LobbyWidget.h"
#include "BoxGame/DataAssets/MapDataAsset.h"
#include "BoxGame/HUD/Lobby/LobbyGameMode.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"

void ALobbyHUD::BeginPlay()
{
	ConfigureLobbyWidget();
}

void ALobbyHUD::ConfigureLobbyWidget()
{
	LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetClass);
	LobbyWidget->AddToViewport();
	LobbyWidget->SetIsFocusable(true);

	LobbyWidget->SetLaunchButton(IsPlayerHost(), false);
	LobbyWidget->OnCancelButtonPressed.BindUObject(this, &ThisClass::CancelGame);
	LobbyWidget->OnReadyButtonPressed.BindUObject(this, &ThisClass::PlayerPressedReady);

	if (IsPlayerHost())
	{
		LobbyWidget->OnLaunchButtonPressed.BindUObject(this, &ThisClass::LaunchGame);
	}

	TMap<FString, UTexture2D*> Maps;

	for (UMapDataAsset* DataAsset : MapDataAssets)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	//LobbyWidget->SetMapGame(Maps, MapSelectorCellClass);
}

bool ALobbyHUD::IsPlayerHost()
{
	ENetMode NetMode = GetNetMode();
	return NetMode == ENetMode::NM_ListenServer || NetMode == ENetMode::NM_DedicatedServer;
}

void ALobbyHUD::UpdatePlayerList(const TArray<struct FLobbyPlayerInfo>& NewPlayersInfo, const struct FLobbyGameInfo& GameInfo)
{
	LobbyWidget->SetPlayersInLobby(NewPlayersInfo, GameInfo);
}

void ALobbyHUD::PlayerCanLaunchGame(bool CanLaunch)
{
	LobbyWidget->SetLaunchButton(IsPlayerHost(), CanLaunch);
}

void ALobbyHUD::LaunchGame(FString MapName)
{
	OnLaunchGameShoot.ExecuteIfBound(MapName);
}

void ALobbyHUD::CancelGame()
{
	OnCancelButtonPressed.ExecuteIfBound();
}

void ALobbyHUD::PlayerPressedReady()
{
	OnReadyButtonPressed.ExecuteIfBound();
}

void ALobbyHUD::ChangeWidgetToLaunch()
{
	if (LobbyWidget) 
	{
		LobbyWidget->ChangeButtonsToLaunch();
	}
}
