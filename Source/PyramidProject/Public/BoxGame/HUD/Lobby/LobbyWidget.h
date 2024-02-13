// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;
class ULobbyPlayerCell;
class UBorder;

DECLARE_DELEGATE(FOnButtonPressed);
DECLARE_DELEGATE_OneParam(FOnButtonOneParamPressed, FString);

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ULobbyWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnButtonPressed OnCancelButtonPressed;
	FOnButtonPressed OnReadyButtonPressed;
	FOnButtonOneParamPressed OnLaunchButtonPressed;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyPlayerCell> LobbyPlayerCellClass;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* NumberOfPlayersText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* BtnReadyText;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnCancel;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnLaunch;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnReady;

	UPROPERTY(meta = (BindWidgetOptional))
	UVerticalBox* VBPlayerList;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* LaunchGameMessage;

	UPROPERTY()
	TArray<ULobbyPlayerCell*> AllLobbyPlayerCells;

	bool ButtonInReady{ true };
	bool ReadyToLaunch{ false };

	const FString ReadyString{ TEXT("Ready") };
	const FString NotReadyString{ TEXT("Not Ready")};

public:
	void SetPlayersInLobby(const TArray<struct FLobbyPlayerInfo>& NewPlayersInfo, const struct FLobbyGameInfo& GameInfo);
	void SetLaunchButton(bool IsHost, bool IsEnabled);
	void ChangeButtonsToLaunch();

protected:
	virtual bool Initialize() override;

private:
	void ConfigureWidget();
	void SetNumberOfPlayers(int32 CurrentCount, int32 MaxCount);
	void CreateLobbyPlayerCell(const FLobbyPlayerInfo PlayerInfo);

	UFUNCTION()
	void CancelButtonClicked();

	UFUNCTION()
	void LaunchButtonClicked();

	UFUNCTION()
	void ReadyButtonClicked();
};
