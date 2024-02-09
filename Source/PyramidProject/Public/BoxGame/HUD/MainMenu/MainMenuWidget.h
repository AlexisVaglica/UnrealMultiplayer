// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UBorder;
class UHorizontalBox;
class UVerticalBox;
class UMapSelectorCell;
class USessionSearchCell;

DECLARE_DELEGATE(FOnButtonPressed);
DECLARE_DELEGATE_OneParam(FOnEnterGameWithMap, FString);

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnButtonPressed OnMenuDismissButtonPressed;
	FOnButtonPressed OnExitGameButtonPressed;
	FOnButtonPressed OnSearchButtonPressed;
	FOnButtonPressed OnConnectionButtonPressed;
	FOnButtonPressed OnHostButtonPressed;
	FOnButtonPressed OnRefreshButtonPressed;

	FOnEnterGameWithMap OnJoinButtonPressed;
	FOnEnterGameWithMap OnLaunchButtonPressed;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSoloGame;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnMultiplayer;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnQuitGame;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnHost;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSearch;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnConnection;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnBack;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnRefresh;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnLaunch;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* MainMenuPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* MultiplayerPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* GameSearchList;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* MapSelectorPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UHorizontalBox* MapSelectorHBox;

	UPROPERTY(meta = (BindWidgetOptional))
	UVerticalBox* SessionSearchVBox;

	UPROPERTY()
	TArray<UMapSelectorCell*> AllMapSelectorCells;

	UPROPERTY()
	TArray<USessionSearchCell*> AllSessionSearchCells;

	FString CurrentMapSelectedName{""};

public:
	TSharedRef<SWidget, ESPMode::ThreadSafe> GetWidgetPrt();

	void SetMapGame(TMap<FString, UTexture2D*> NewMaps, TSubclassOf<UMapSelectorCell> MapCellClass);
	void SetSessionResults(TArray<FString> SessionResults, TSubclassOf<USessionSearchCell> SessionCellClass);

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void SoloGameBtnClicked();

	UFUNCTION()
	void MultiplayerBtnClicked();

	UFUNCTION()
	void QuitGameBtnClicked();

	UFUNCTION()
	void SearchBtnClicked();

	UFUNCTION()
	void ConnectionBtnClicked();

	UFUNCTION()
	void BackBtnClicked();

	UFUNCTION()
	void HostBtnClicked();

	UFUNCTION()
	void LaunchBtnClicked();

	UFUNCTION()
	void RefreshBtnClicked();

	UFUNCTION()
	void MapSelected(FString MapSelectedName);

	UFUNCTION()
	void SessionSelected(FString SessionId);

	void MenuDismiss();
	void ChangeSoloGameVisibility(bool IsSoloGamePressed);
	void ChangeSearchVisibility(bool IsSearchPressed);
	void CreateMapSelectCell(FString MapName, UTexture2D* MapImage, TSubclassOf<UMapSelectorCell> MapCellClass);
	void CreateSessionResultCell(FString SessionId, FString GameMapName, int32 CurrentPlayersCount, TSubclassOf<USessionSearchCell> SessionCellClass);
	void ShowOrDismissMessage(bool isShowing);
};
