// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/MainMenu/SessionSearchCell.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionSearchCell::ConfigureCell(FString SessionId, FString PlayerName, int32 CurrentPlayersInSession, int32 MaxPlayersForSession)
{
	CurrentSessionId = SessionId;

	if (TextPlayerName)
	{
		FText NameText = FText::FromString(PlayerName);
		TextPlayerName->SetText(NameText);
	}

	if (TextPlayers) 
	{
		FString PlayersCurrent = FString::Printf(TEXT("Players In Session: %d / %d"), CurrentPlayersInSession, MaxPlayersForSession);
		FText PlayersText = FText::FromString(PlayersCurrent);
		TextPlayers->SetText(PlayersText);
	}

	if (BtnJoinSession) 
	{
		BtnJoinSession->OnClicked.AddDynamic(this, &ThisClass::JoinSessionButtonClicked);
	}
}

void USessionSearchCell::JoinSessionButtonClicked() 
{
	OnSessionSelected.ExecuteIfBound(CurrentSessionId);
}
