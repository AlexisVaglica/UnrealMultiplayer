// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionSearchCell.generated.h"

DECLARE_DELEGATE_OneParam(FOnSessionSelected, FString);

class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API USessionSearchCell : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnSessionSelected OnSessionSelected;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnJoinSession;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextId;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextMapName;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextPlayers;

private:
	FString CurrentSessionId{ "" };

public:
	void ConfigureCell(FString Id, FString MapName, int32 CurrentPlayersInSession);

private:
	void JoinSessionButtonClicked();
};
