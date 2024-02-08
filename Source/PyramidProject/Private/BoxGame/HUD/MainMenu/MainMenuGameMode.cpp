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

	MainMenuWidget->OnLaunchButtonPressed.BindUObject(this, &ThisClass::LaunchSoloGame);
	MainMenuWidget->OnExitGameButtonPressed.BindUObject(this, &ThisClass::QuitGame);
	MainMenuWidget->OnHostButtonPressed.BindUObject(this, &ThisClass::LaunchHostGame);

	TMap<FString, UTexture2D*> Maps;

	for (UMultiplayerDataAsset* DataAsset : MultiplayerMapData)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	MainMenuWidget->SetMapGame(Maps, MapSelectorCellClass);
}

void AMainMenuGameMode::LaunchHostGame()
{
	FString LobbyMapPath = FString::Printf(TEXT("%s?listen"), *LobbyMap.ToSoftObjectPath().ToString());

	UGameplayStatics::OpenLevel(GetWorld(), FName(LobbyMapPath), true);
}

void AMainMenuGameMode::LaunchSoloGame(FString MapName)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}