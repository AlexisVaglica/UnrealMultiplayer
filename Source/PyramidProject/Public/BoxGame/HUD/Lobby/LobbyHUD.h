// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class ULobbyWidget;
class UMapDataAsset;
class UMapSelectorCell;
class APlayerController;

DECLARE_DELEGATE_OneParam(FOnButtonOneParamPressed, FString);
DECLARE_DELEGATE(FOnButtonPressed);

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
	FOnButtonPressed OnReadyButtonPressed;
	FOnButtonPressed OnCancelButtonPressed;
	FOnButtonPressed OnLaunchGameSelected;
	FOnButtonOneParamPressed OnMapWasSelected;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMapSelectorCell> MapSelectorCellClass;

	UPROPERTY(EditAnywhere)
	TArray<UMapDataAsset*> MapsData;

	UPROPERTY()
	ULobbyWidget* LobbyWidget;

public:
	void UpdatePlayerList(const TArray<struct FLobbyPlayerInfo>& NewPlayersInfo, const struct FLobbyGameInfo& GameInfo);
	void PlayerCanLaunchGame(bool CanLaunch);
	void ChangeWidgetToLaunch();
	void ChangeMapSelected(const FString& MapName);

protected:
	virtual void BeginPlay() override;

private:
	bool IsPlayerHost();
	void ConfigureLobbyWidget();
	void LaunchGame();
	void CancelGame();
	void PlayerPressedReady();

	UFUNCTION()
	void MapSelected(FString MapName);
};
