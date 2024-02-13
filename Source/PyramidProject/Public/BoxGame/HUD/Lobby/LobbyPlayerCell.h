// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerCell.generated.h"

class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ULobbyPlayerCell : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextPlayerName;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ImageIsReady;

public:
	void ConfigurePlayerCell(struct FLobbyPlayerInfo LobbyPlayerInfo);
};
