// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/Lobby/LobbyGameMode.h"
#include "BoxGame/HUD/Lobby/LobbyWidget.h"
#include "BoxGame/DataAssets/MapDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void ALobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	LobbyWidget = CreateWidget<ULobbyWidget>(GetWorld(), LobbyWidgetClass);
	LobbyWidget->AddToViewport();
	LobbyWidget->SetIsFocusable(true);

	ConfigureLobbyWidget();
}

void ALobbyGameMode::ConfigureLobbyWidget()
{
	if (!LobbyWidget)
	{
		return;
	}

	//LobbyWidget->OnLaunchButtonPressed.BindUObject(this, &ThisClass::LaunchGame);
	//LobbyWidget->OnExitGameButtonPressed.BindUObject(this, &ThisClass::QuitGame);

	TMap<FString, UTexture2D*> Maps;

	for (UMapDataAsset* DataAsset : MapDataAssets)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	//LobbyWidget->SetMapGame(Maps, MapSelectorCellClass);
}

void ALobbyGameMode::LaunchGame(FString MapName)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void ALobbyGameMode::QuitGame()
{
	//Go To Main Menu
	//UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);

}