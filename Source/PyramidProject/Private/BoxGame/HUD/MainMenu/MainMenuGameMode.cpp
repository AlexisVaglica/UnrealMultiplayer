// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/MainMenuGameMode.h"
#include "BoxGame/HUD/MainMenu/MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "MultiplayerSession/Public/Multiplayer/GameData/MultiplayerDataAsset.h"

void AMainMenuGameMode::BeginPlay() 
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetIsFocusable(true);

	ConfigureMainMenuWidget();
	ConfigureOnlineSubsystem();
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
	MainMenuWidget->OnRefreshButtonPressed.BindUObject(this, &ThisClass::RefreshGameList);
	MainMenuWidget->OnSearchButtonPressed.BindUObject(this, &ThisClass::RefreshGameList);

	TMap<FString, UTexture2D*> Maps;

	for (UMultiplayerDataAsset* DataAsset : MultiplayerMapData)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	MainMenuWidget->SetMapGame(Maps, MapSelectorCellClass);
}

void AMainMenuGameMode::ConfigureOnlineSubsystem()
{
	if (MultiplayerSession) 
	{
		MultiplayerSession->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::CreateSessionComplete);
	}
}

void AMainMenuGameMode::LaunchSoloGame(FString MapName)
{
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void AMainMenuGameMode::LaunchHostGame()
{
	//ToDo: Create Session 
	if (MultiplayerSession) 
	{
		UMultiplayerDataAsset* DataAsset = MultiplayerMapData[0];
		MultiplayerSession->CreateSession(DataAsset);
	}
}

void AMainMenuGameMode::RefreshGameList() 
{
	MultiplayerSession->FindSessions(MaxGamesSearchCount);
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

void AMainMenuGameMode::ShowErrorMessage(FString ErrorMessage)
{
	//ToDo: Show Error Message in Widget
}

void AMainMenuGameMode::CreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful) 
	{
		FString LobbyMapPath = FString::Printf(TEXT("%s?listen"), *LobbyMap.ToSoftObjectPath().ToString());
		UGameplayStatics::OpenLevel(GetWorld(), FName(LobbyMapPath), true);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ERROR - CreateSessionComplete"));
		ShowErrorMessage(TEXT("Error to Create Session"));
	}
}
