// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/SessionSearchCell.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"

void USessionSearchCell::ConfigureCell(FString Id, FString MapName, int32 CurrentPlayersInSession)
{
	CurrentSessionId = Id;

	if (TextId) 
	{
		FText IDText = FText::FromString(Id);
		TextId->SetText(IDText);
	}

	if (TextMapName)
	{
		FText MapNameText = FText::FromString(MapName);
		TextMapName->SetText(MapNameText);
	}

	if (TextPlayers) 
	{
		FString PlayersCurrent = FString::Printf(TEXT("Players In Session: %s / 4"), CurrentPlayersInSession);
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
