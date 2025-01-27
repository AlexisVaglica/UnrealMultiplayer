// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/MainMenu/MainMenuWidget.h"
#include "BoxGame/MainMenu/MapSelectorCell.h"
#include "BoxGame/MainMenu/SessionSearchCell.h"
#include "BoxGame/MainMenu/MainMenuGameMode.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Overlay.h"
#include "Components/CircularThrobber.h"
#include "Components/EditableTextBox.h"


bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	ConfigureWidget();

	return true;
}

void UMainMenuWidget::ConfigureWidget()
{
	if (BtnSoloGame)
	{
		BtnSoloGame->OnClicked.AddDynamic(this, &ThisClass::SoloGameBtnClicked);
	}

	if (BtnMultiplayer)
	{
		BtnMultiplayer->OnClicked.AddDynamic(this, &ThisClass::MultiplayerBtnClicked);
	}

	if (BtnQuitGame)
	{
		BtnQuitGame->OnClicked.AddDynamic(this, &ThisClass::QuitGameBtnClicked);
	}

	if (BtnHost)
	{
		BtnHost->OnClicked.AddDynamic(this, &ThisClass::HostBtnClicked);
	}

	if (BtnSearch)
	{
		BtnSearch->OnClicked.AddDynamic(this, &ThisClass::SearchBtnClicked);
	}

	if (BtnConnection)
	{
		BtnConnection->OnClicked.AddDynamic(this, &ThisClass::ConnectionBtnClicked);
	}

	if (BtnBack)
	{
		BtnBack->OnClicked.AddDynamic(this, &ThisClass::BackBtnClicked);
	}

	if (BtnLaunch)
	{
		BtnLaunch->OnClicked.AddDynamic(this, &ThisClass::LaunchBtnClicked);
		BtnLaunch->SetIsEnabled(false);
	}

	if (BtnRefresh)
	{
		BtnRefresh->OnClicked.AddDynamic(this, &ThisClass::RefreshBtnClicked);
	}

	if (BtnGeneralMessage)
	{
		BtnGeneralMessage->OnClicked.AddDynamic(this, &ThisClass::GeneralMessageConfirmBtnClicked);
		ShowOrDismissGeneralMessage(false);
	}
}

void UMainMenuWidget::HostBtnClicked()
{
	BtnHost->SetIsEnabled(false);

	OnHostButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::LaunchBtnClicked()
{
	if (CurrentMapSelectedName != "")
	{
		OnLaunchButtonPressed.ExecuteIfBound(CurrentMapSelectedName);
	}
}

void UMainMenuWidget::MenuDismiss()
{
	OnHostButtonPressed.Unbind();
	OnExitGameButtonPressed.Unbind();
	OnSearchButtonPressed.Unbind();
	OnConnectionButtonPressed.Unbind();
	OnJoinButtonPressed.Unbind();
	OnLaunchButtonPressed.Unbind();
	OnRefreshButtonPressed.Unbind();

	OnMenuDismissButtonPressed.ExecuteIfBound();
	OnMenuDismissButtonPressed.Unbind();
}

void UMainMenuWidget::ChangeSoloGameVisibility(bool IsSoloGamePressed)
{
	ESlateVisibility SoloGameSlateVisibility = IsSoloGamePressed ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	BtnSoloGame->SetIsEnabled(!IsSoloGamePressed);
	BtnLaunch->SetVisibility(SoloGameSlateVisibility);

	MapSelectorPanel->SetVisibility(SoloGameSlateVisibility);
}

void UMainMenuWidget::ChangeSearchVisibility(bool IsSearchPressed) 
{
	ESlateVisibility SearchSlateVisibility = IsSearchPressed ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	BtnSearch->SetIsEnabled(!IsSearchPressed);
	BtnRefresh->SetVisibility(SearchSlateVisibility);
	GameSearchList->SetVisibility(SearchSlateVisibility);
}

void UMainMenuWidget::NativeDestruct()
{
	MenuDismiss();
	Super::NativeDestruct();
}

void UMainMenuWidget::SoloGameBtnClicked()
{
	ChangeSoloGameVisibility(true);
}

void UMainMenuWidget::MultiplayerBtnClicked()
{
	ChangeSoloGameVisibility(false);
	ChangeSearchVisibility(false);

	MainMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	MultiplayerPanel->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::QuitGameBtnClicked()
{
	OnExitGameButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::SearchBtnClicked()
{
	ChangeSearchVisibility(true);
	OnSearchButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::ConnectionBtnClicked()
{
	OnConnectionButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::BackBtnClicked()
{
	MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
	MultiplayerPanel->SetVisibility(ESlateVisibility::Hidden);

	ChangeSoloGameVisibility(false);
	ChangeSearchVisibility(false);
}

void UMainMenuWidget::RefreshBtnClicked() 
{
	OnRefreshButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::GeneralMessageConfirmBtnClicked()
{
	ShowOrDismissGeneralMessage(false);
}

void UMainMenuWidget::MapSelected(FString MapSelectedName)
{
	CurrentMapSelectedName = MapSelectedName;

	for (auto MapCell : AllMapSelectorCells)
	{
		if (!MapCell->CompareMapName(CurrentMapSelectedName))
		{
			MapCell->DeselectCell();
		}
	}

	if (BtnLaunch)
	{
		BtnLaunch->SetIsEnabled(CurrentMapSelectedName != "");
	}
}

void UMainMenuWidget::SessionSelected(FString SessionId)
{
	OnJoinButtonPressed.ExecuteIfBound(SessionId);
}

TSharedRef<SWidget, ESPMode::ThreadSafe> UMainMenuWidget::GetWidgetPrt()
{
	return TakeWidget();
}

void UMainMenuWidget::SetPlayerName(FString PlayerName)
{
	if (TextBoxPlayerName) 
	{
		FText PlayerNameText = FText::FromString(PlayerName);
		TextBoxPlayerName->SetText(PlayerNameText);
	}
}

FString UMainMenuWidget::GetPlayerName()
{
	return TextBoxPlayerName->GetText().ToString();
}

void UMainMenuWidget::SetMapGame(TMap<FString, UTexture2D*> NewMaps, TSubclassOf<UMapSelectorCell> MapCellClass)
{
	if (NewMaps.Num() > 0)
	{
		TArray<FString> MapNames;
		NewMaps.GetKeys(MapNames);

		for (FString MapName : MapNames)
		{
			CreateMapSelectCell(MapName, NewMaps[MapName], MapCellClass);
		}
	}
}

void UMainMenuWidget::SetSessionResults(TArray<FSessionGameInfo> SessionResults, TSubclassOf<USessionSearchCell> SessionCellClass)
{
	if (SessionResults.Num() > 0)
	{
		for (FSessionGameInfo Session : SessionResults)
		{
			CreateSessionResultCell(Session, SessionCellClass);
		}
	}
	else
	{
		CurrentSessionText->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainMenuWidget::CreateMapSelectCell(FString MapName, UTexture2D* MapImage, TSubclassOf<UMapSelectorCell> MapCellClass)
{
	if (MapSelectorHBox)
	{
		UMapSelectorCell* MapCellWidget = CreateWidget<UMapSelectorCell>(this, MapCellClass);
		MapCellWidget->ConfigureCell(MapName, MapImage);
		MapCellWidget->OnMapButtonPressed.BindUObject(this, &ThisClass::MapSelected);
		MapSelectorHBox->AddChildToHorizontalBox(MapCellWidget);

		AllMapSelectorCells.Add(MapCellWidget);
	}
}

void UMainMenuWidget::CreateSessionResultCell(FSessionGameInfo SessionInfo, TSubclassOf<USessionSearchCell> SessionCellClass)
{
	if (SessionSearchVBox)
	{
		USessionSearchCell* SessionCellWidget = CreateWidget<USessionSearchCell>(this, SessionCellClass);
		SessionCellWidget->ConfigureCell(SessionInfo.SessionId, SessionInfo.OwnerName, SessionInfo.CurrentPlayersCount, SessionInfo.MaxPlayersCount);
		SessionCellWidget->OnSessionSelected.BindUObject(this, &ThisClass::SessionSelected);
		SessionSearchVBox->AddChildToVerticalBox(SessionCellWidget);

		AllSessionSearchCells.Add(SessionCellWidget);
	}
}

void UMainMenuWidget::ShowOrDismissGeneralMessage(bool IsShowing, FString NewText, bool IsButtonShowing)
{
	if(GeneralMessageOverlay) 
	{
		ESlateVisibility MessageOverlayVisibility = IsShowing ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		GeneralMessageOverlay->SetVisibility(MessageOverlayVisibility);

		if (TextGeneralMessage) 
		{
			FText TextToShow = FText::FromString(NewText);
			TextGeneralMessage->SetText(TextToShow);
		}

		if (BtnGeneralMessage) 
		{
			ESlateVisibility BtnVisibility = IsButtonShowing ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
			BtnGeneralMessage->SetVisibility(BtnVisibility);
		}
	}
}

void UMainMenuWidget::ChangeConnectionType(bool bIsLan)
{
	if (ConnectionText) 
	{
		FString ConnectionString = bIsLan ? FString(TEXT("LAN")) : FString(TEXT("STEAM"));
		FText NewConnectionText = FText::FromString(ConnectionString);
		ConnectionText->SetText(NewConnectionText);
	}
}

void UMainMenuWidget::StartSessionSearch()
{
	if (CurrentSessionText)
	{
		CurrentSessionText->SetVisibility(ESlateVisibility::Hidden);
	}

	if (BtnRefresh) 
	{
		BtnRefresh->SetIsEnabled(false);
	}

	if (SessionSearchLoader) 
	{
		SessionSearchLoader->SetVisibility(ESlateVisibility::Visible);
	}

	if (AllSessionSearchCells.Num() > 0)
	{
		for (USessionSearchCell* SessionCell : AllSessionSearchCells)
		{
			SessionCell->RemoveFromParent();
			SessionCell->RemoveFromRoot();
		}

		AllSessionSearchCells.Empty();
	}
}

void UMainMenuWidget::StopSessionSearch()
{
	if (BtnRefresh)
	{
		BtnRefresh->SetIsEnabled(true);
	}

	if (SessionSearchLoader)
	{
		SessionSearchLoader->SetVisibility(ESlateVisibility::Hidden);
	}
}
