// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyWidget.generated.h"

class UTextBlock;
class UButton;
class UVerticalBox;
class UHorizontalBox;
class ULobbyPlayerCell;
class UBorder;
class UMapDataAsset;
class UMapSelectorCell;

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
	FOnButtonPressed OnLaunchButtonPressed;
	FOnButtonOneParamPressed OnMapButtonPressed;

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
	UHorizontalBox* MapSelectorHBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* LaunchGameMessage;

	UPROPERTY()
	TArray<ULobbyPlayerCell*> AllLobbyPlayerCells;

	UPROPERTY()
	TArray<UMapSelectorCell*> AllMapSelectorCells;

	bool ButtonInReady{ true };
	bool MapWasSelected{ false };
	bool IsLaunchingGame{ false };

	FString CurrentMapSelectedName{ "" };

	const FString ReadyString{ TEXT("Ready") };
	const FString NotReadyString{ TEXT("Not Ready")};

public:
	void SetPlayersInLobby(const TArray<struct FLobbyPlayerInfo>& NewPlayersInfo, const struct FLobbyGameInfo& GameInfo);
	void SetLaunchButton(bool IsHost, bool IsEnabled);
	void ChangeButtonsToLaunch();

	void UpdateMaps(TArray<UMapDataAsset*> NewMaps, TSubclassOf<UMapSelectorCell> MapCellClass, bool IsHost);
	void ReplicateMapSelected(const FString& MapName);

protected:
	virtual bool Initialize() override;

private:
	void ConfigureWidget();
	void SetNumberOfPlayers(int32 CurrentCount, int32 MaxCount);
	void CreateLobbyPlayerCell(const FLobbyPlayerInfo PlayerInfo);

	void CreateMapSelectCell(FString MapName, UTexture2D* MapImage, TSubclassOf<UMapSelectorCell> MapCellClass, bool IsHost);
	void MapSelected(FString MapSelectedName);

	UFUNCTION()
	void CancelButtonClicked();

	UFUNCTION()
	void LaunchButtonClicked();

	UFUNCTION()
	void ReadyButtonClicked();

};
