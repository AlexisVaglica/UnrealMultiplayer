// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/MainMenuGameMode.h"
#include "BoxGame/HUD/MainMenu/MainMenuWidget.h"
#include "BoxGame/DataAssets/MapDataAsset.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h" 

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
	MainMenuWidget->OnJoinButtonPressed.BindUObject(this, &ThisClass::JoinSessionGame);

	TMap<FString, UTexture2D*> Maps;

	for (UMapDataAsset* DataAsset : MapData)
	{
		TTuple<FString, UTexture2D*> Map = TTuple<FString, UTexture2D*>(DataAsset->GameMapName.ToString(), DataAsset->MapImage.Get());
		Maps.Add(Map);
	}

	MainMenuWidget->SetMapGame(Maps, MapSelectorCellClass);
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
	UGameplayStatics::OpenLevel(GetWorld(), FName(MapName), true);
}

void AMainMenuGameMode::LaunchHostGame()
{
	if (MultiplayerSession && MultiplayerData)
	{
		MultiplayerSession->CreateSession(MultiplayerData);
	}
}

void AMainMenuGameMode::RefreshGameList() 
{
	MultiplayerSession->FindSessions(MaxGamesSearchCount);
	MainMenuWidget->StartSessionSearch();
}

void AMainMenuGameMode::JoinSessionGame(FString SessionId)
{
	if (MultiplayerSession) 
	{
		MultiplayerSession->JoinSession(SessionId);
		MainMenuWidget->ShowOrDismissGeneralMessage(true, TEXT("Connecting to Session Host..."), false);
	}
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}

void AMainMenuGameMode::ShowErrorMessage(FString ErrorMessage)
{
	MainMenuWidget->ShowOrDismissGeneralMessage(true, ErrorMessage);
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
		ShowErrorMessage(ErrorCreateSessionMessage);
	}
}

void AMainMenuGameMode::FindSessionsComplete(const TArray<FString>& SessionIdResults, bool bWasSuccess)
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
