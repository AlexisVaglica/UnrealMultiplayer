// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Lobby/LobbyWidget.h"
#include "BoxGame/Lobby/LobbyGameMode.h"
#include "BoxGame/Lobby/LobbyPlayerCell.h"
#include "BoxGame/MainMenu/MapSelectorCell.h"
#include "BoxGame/DataAssets/MapDataAsset.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Border.h"

bool ULobbyWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	ConfigureWidget();

	return true;
}

void ULobbyWidget::ConfigureWidget()
{
	if (BtnCancel)
	{
		BtnCancel->OnClicked.AddDynamic(this, &ThisClass::CancelButtonClicked);
	}

	if (BtnLaunch) 
	{
		BtnLaunch->OnClicked.AddDynamic(this, &ThisClass::LaunchButtonClicked);
	}

	if (BtnReady) 
	{
		BtnReady->OnClicked.AddDynamic(this, &ThisClass::ReadyButtonClicked);
	}
}

void ULobbyWidget::CancelButtonClicked()
{
	OnCancelButtonPressed.ExecuteIfBound();
}

void ULobbyWidget::LaunchButtonClicked()
{
	OnLaunchButtonPressed.ExecuteIfBound();
}

void ULobbyWidget::ReadyButtonClicked()
{
	ButtonInReady = !ButtonInReady;
	FText ReadyText = ButtonInReady ? FText::FromString(ReadyString) : FText::FromString(NotReadyString);
	BtnReadyText->SetText(ReadyText);

	OnReadyButtonPressed.ExecuteIfBound();
}

void ULobbyWidget::SetNumberOfPlayers(int32 CurrentCount, int32 MaxCount)
{
	if (NumberOfPlayersText) 
	{
		FString PlayersCurrent = FString::Printf(TEXT("Players %d/%d"), CurrentCount, MaxCount);
		FText PlayersText = FText::FromString(PlayersCurrent);
		NumberOfPlayersText->SetText(PlayersText);
	}
}

void ULobbyWidget::SetPlayersInLobby(const TArray<FLobbyPlayerInfo>& NewPlayersInfo, const struct FLobbyGameInfo& GameInfo)
{
	SetNumberOfPlayers(NewPlayersInfo.Num(), GameInfo.MaxNumberOfPlayerInMatch);

	if (AllLobbyPlayerCells.Num() > 0)
	{
		for (auto PlayerCell : AllLobbyPlayerCells) 
		{
			PlayerCell->RemoveFromParent();
			PlayerCell->RemoveFromRoot();
		}

		AllLobbyPlayerCells.Empty();
	}

	for (auto PlayerInfo : NewPlayersInfo) 
	{
		CreateLobbyPlayerCell(PlayerInfo);
	}
}

void ULobbyWidget::SetLaunchButton(bool IsHost, bool IsEnabled)
{
	ESlateVisibility ButonVisibility = IsHost ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	BtnLaunch->SetVisibility(ButonVisibility);

	bool bBtnLaunchIsEnabled = IsEnabled && !IsLaunchingGame && MapWasSelected;
	BtnLaunch->SetIsEnabled(bBtnLaunchIsEnabled);
}

void ULobbyWidget::CreateLobbyPlayerCell(const FLobbyPlayerInfo PlayerInfo)
{
	if (VBPlayerList)
	{
		ULobbyPlayerCell* PlayerCellWidget = CreateWidget<ULobbyPlayerCell>(this, LobbyPlayerCellClass);
		PlayerCellWidget->ConfigurePlayerCell(PlayerInfo);
		VBPlayerList->AddChildToVerticalBox(PlayerCellWidget);

		AllLobbyPlayerCells.Add(PlayerCellWidget);
	}
}

void ULobbyWidget::ChangeButtonsToLaunch()
{
	IsLaunchingGame = true;

	LaunchGameMessage->SetVisibility(ESlateVisibility::Visible);
	BtnLaunch->SetIsEnabled(false);
	BtnCancel->SetIsEnabled(false);
	BtnReady->SetIsEnabled(false);
}

void ULobbyWidget::UpdateMaps(TArray<UMapDataAsset*> NewMaps, TSubclassOf<UMapSelectorCell> MapCellClass, bool IsHost)
{
	if (NewMaps.Num() > 0)
	{
		for (auto MapData : NewMaps)
		{
			auto MapName = MapData->GameMapName.ToString();
			auto MapImage = MapData->MapImage.Get();
			CreateMapSelectCell(MapName, MapImage, MapCellClass, IsHost);
		}
	}
}

void ULobbyWidget::ReplicateMapSelected(const FString& MapName)
{
	for (auto MapCell : AllMapSelectorCells)
	{
		if (!MapCell->CompareMapName(MapName))
		{
			MapCell->DeselectCell();
		}
		else
		{
			MapCell->SelectMap();
		}
	}
}

void ULobbyWidget::CreateMapSelectCell(FString MapName, UTexture2D* MapImage, TSubclassOf<UMapSelectorCell> MapCellClass, bool IsHost)
{
	if (MapSelectorHBox)
	{
		UMapSelectorCell* MapCellWidget = CreateWidget<UMapSelectorCell>(this, MapCellClass);
		MapCellWidget->ConfigureCell(MapName, MapImage);
		MapCellWidget->ConfigureCellOnline(IsHost);

		if (IsHost) 
		{
			MapCellWidget->OnMapButtonPressed.BindUObject(this, &ThisClass::MapSelected);
		}

		MapSelectorHBox->AddChildToHorizontalBox(MapCellWidget);
		AllMapSelectorCells.Add(MapCellWidget);
	}
}

void ULobbyWidget::MapSelected(FString MapSelectedName)
{
	CurrentMapSelectedName = MapSelectedName;
	MapWasSelected = true;

	for (auto MapCell : AllMapSelectorCells)
	{
		if (!MapCell->CompareMapName(CurrentMapSelectedName))
		{
			MapCell->DeselectCell();
		}
	}

	OnMapButtonPressed.ExecuteIfBound(CurrentMapSelectedName);
}