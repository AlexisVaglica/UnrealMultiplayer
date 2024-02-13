// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/Lobby/LobbyWidget.h"
#include "BoxGame/HUD/Lobby/LobbyGameMode.h"
#include "BoxGame/HUD/Lobby/LobbyPlayerCell.h"
#include "Components/VerticalBox.h"
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
	OnLaunchButtonPressed.ExecuteIfBound("PortMap");
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

	bool bBtnLaunchIsEnabled = IsEnabled && !ReadyToLaunch;
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
	ReadyToLaunch = true;

	LaunchGameMessage->SetVisibility(ESlateVisibility::Visible);
	BtnLaunch->SetIsEnabled(false);
	BtnCancel->SetIsEnabled(false);
	BtnReady->SetIsEnabled(false);
}
