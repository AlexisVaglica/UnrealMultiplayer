// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/MainMenuGameMode.h"
#include "BoxGame/HUD/MainMenu/MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSession/Public/Multiplayer/GameData/MultiplayerDataAsset.h"

void AMainMenuGameMode::BeginPlay() 
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetIsFocusable(true);

	ConfigureMainMenuWidget();
}

void AMainMenuGameMode::ConfigureMainMenuWidget()
{
	if (!MainMenuWidget) 
	{
		return;
	}

	MainMenuWidget->OnLaunchButtonPressed.BindUObject(this, &ThisClass::LaunchGame);
	MainMenuWidget->OnExitGameButtonPressed.BindUObject(this, &ThisClass::QuitGame);
	MainMenuWidget->OnSoloGameButtonPressed.BindUObject(this, &ThisClass::SoloGame);

	TMap<FString, UTexture2D*> Maps;

	for (UMultiplayerDataAsset* DataAsset : MultiplayerMapData)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	MainMenuWidget->SetMapGame(Maps, MapSelectorCellClass);
}

void AMainMenuGameMode::LaunchGame(FString MapName)
{
	//ToDo: Send To Lobby
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void AMainMenuGameMode::SoloGame(FString MapName)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}