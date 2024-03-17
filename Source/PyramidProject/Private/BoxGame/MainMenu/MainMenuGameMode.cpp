// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/MainMenu/MainMenuGameMode.h"
#include "BoxGame/MainMenu/MainMenuWidget.h"
#include "BoxGame/DataAssets/MapDataAsset.h"
#include "BoxGame/Utils/ReadWriteJsonFile.h"
#include "BoxGame/Utils/PlayerSettings.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineSessionSettings.h" 
#include "GameFramework/PlayerState.h"
#include "Multiplayer/GameData/MultiplayerDataAsset.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h" 	

void AMainMenuGameMode::BeginPlay() 
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	MainMenuWidget->SetIsFocusable(true);

	ConfigurePlayerSettings();
	ConfigureMainMenuWidget();
	ConfigureOnlineSubsystem();
}

void AMainMenuGameMode::ConfigurePlayerSettings()
{
	PlayerSettings = NewObject<UPlayerSettings>();

	if (PlayerSettings)
	{
		bool bSuccess = false;
		FString Message;

		UReadWriteJsonFile::ReadJson(PlayerSettings->PlayerSettingsPath, bSuccess, Message, PlayerSettings);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}
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
	MainMenuWidget->OnJoinButtonPressed.BindUObject(this, &ThisClass::JoinSessionGame);
	MainMenuWidget->OnConnectionButtonPressed.BindUObject(this, &ThisClass::ChangeConnection);

	TMap<FString, UTexture2D*> Maps;

	for (UMapDataAsset* DataAsset : MapData)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	MainMenuWidget->SetMapGame(Maps, MapSelectorCellClass);

	if (PlayerSettings) 
	{
		FString PlayerName = PlayerSettings->LocalPlayerInfo.LocalName;
		MainMenuWidget->SetPlayerName(PlayerName);
	}
}

void AMainMenuGameMode::ConfigureOnlineSubsystem()
{
	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance)
	{
		MultiplayerSession = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();

		if (MultiplayerSession)
		{
			MultiplayerSession->MultiplayerOnCreateSessionComplete.AddDynamic(this, &ThisClass::CreateSessionComplete);
			MultiplayerSession->MultiplayerOnFindSessionComplete.AddUObject(this, &ThisClass::FindSessionsComplete);
			MultiplayerSession->MultiplayerOnJoinSessionComplete.AddUObject(this, &ThisClass::JoinSessionComplete);
		}
	}
}

void AMainMenuGameMode::LaunchSoloGame(FString MapName)
{
	SavePlayerName();
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void AMainMenuGameMode::LaunchHostGame()
{
	if (MultiplayerSession && MultiplayerData)
	{
		SavePlayerName();

		if (PlayerSettings) 
		{
			MultiplayerData->SessionName = PlayerSettings->LocalPlayerInfo.LocalName;
		}

		MultiplayerSession->CreateSession(MultiplayerData);
	}
}

void AMainMenuGameMode::RefreshGameList() 
{
	MultiplayerSession->FindSessions(MaxGamesSearchCount, MultiplayerData->IsLanMatch);
	MainMenuWidget->StartSessionSearch();
}

void AMainMenuGameMode::JoinSessionGame(FString SessionId)
{
	if (MultiplayerSession && MultiplayerData)
	{
		SavePlayerName();

		MultiplayerSession->JoinSession(SessionId, MultiplayerData->IsLanMatch);
		MainMenuWidget->ShowOrDismissGeneralMessage(true, JoinSessionMessage, false);
	}
}

void AMainMenuGameMode::ChangeConnection()
{
	if (MultiplayerData && MultiplayerSession) 
	{
		if (MultiplayerData->IsLanMatch && MultiplayerSession->IsSteamConnection())
		{
			MultiplayerData->IsLanMatch = false;
			MainMenuWidget->ChangeConnectionType(false);
		}
		else
		{
			MultiplayerData->IsLanMatch = true;
			MainMenuWidget->ChangeConnectionType(true);
		}
	}
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

void AMainMenuGameMode::SavePlayerName()
{
	if (PlayerSettings)
	{
		PlayerSettings->LocalPlayerInfo.LocalName = MainMenuWidget->GetPlayerName();

		bool bSuccess = false;
		FString Message;

		UReadWriteJsonFile::WriteJson(PlayerSettings->PlayerSettingsPath, PlayerSettings, bSuccess, Message);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Message);
	}
}

void AMainMenuGameMode::ShowErrorMessage(FString ErrorMessage)
{
	MainMenuWidget->ShowOrDismissGeneralMessage(true, ErrorMessage);
}

void AMainMenuGameMode::CreateSessionComplete(bool bWasSuccessful)
{
	if (bWasSuccessful) 
	{
		FString LobbyMapPath = FString::Printf(TEXT("%s?listen"), *LobbyMap.GetLongPackageName());

		if (MultiplayerData && MultiplayerData->IsLanMatch)
		{
			LobbyMapPath.Append("?bIsLanMatch=1");
		}

		UWorld* World = GetWorld();

		if (World) 
		{
			World->ServerTravel(LobbyMapPath);
		}
	}
	else
	{
		ShowErrorMessage(ErrorCreateSessionMessage);
	}
}

void AMainMenuGameMode::FindSessionsComplete(const TArray<FSessionGameInfo>& SessionIdResults, bool bWasSuccess)
{
	if (bWasSuccess)
	{
		MainMenuWidget->SetSessionResults(SessionIdResults, SessionSearchCellClass);
	}
	else
	{
		ShowErrorMessage(ErrorFindSessionsMessage);
	}

	MainMenuWidget->StopSessionSearch();
}

void AMainMenuGameMode::JoinSessionComplete(bool bWasSuccessful)
{
	MainMenuWidget->ShowOrDismissGeneralMessage(false);

	if (MultiplayerSession && bWasSuccessful) 
	{
		FString AddressToTravel = MultiplayerSession->GetResolvedConnectString();

		if (!AddressToTravel.IsEmpty())
		{
			APlayerController* PlayerController = GetGameInstance()->GetFirstLocalPlayerController();
			if (PlayerController) 
			{
				PlayerController->ClientTravel(AddressToTravel, ETravelType::TRAVEL_Absolute);
				return;
			}
		}
	}

	ShowErrorMessage(ErrorJoinSessionsMessage);
}
