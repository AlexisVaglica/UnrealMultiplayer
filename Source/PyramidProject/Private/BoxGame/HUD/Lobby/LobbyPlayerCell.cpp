// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/Lobby/LobbyPlayerCell.h"
#include "BoxGame/HUD/Lobby/LobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

void ULobbyPlayerCell::ConfigurePlayerCell(FLobbyPlayerInfo LobbyPlayerInfo)
{
	if (TextPlayerName)
	{
		FText NameText = FText::FromString(LobbyPlayerInfo.PlayerName);
		TextPlayerName->SetText(NameText);
	}

	if (ImageIsReady) 
	{
		FLinearColor NewColor = LobbyPlayerInfo.bPlayerReadyState ? FLinearColor::Green : FLinearColor::Red;
		ImageIsReady->SetColorAndOpacity(NewColor);
	}
}
