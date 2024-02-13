// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LobbyHUD.generated.h"

class ULobbyWidget;
class UMapDataAsset;
class UMapSelectorCell;
class APlayerController;

DECLARE_DELEGATE_OneParam(FOnLaunchGameShoot, FString);
DECLARE_DELEGATE(FOnButtonPressed);

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ALobbyHUD : public AHUD
{
	GENERATED_BODY()

public:
	FOnLaunchGameShoot OnLaunchGameShoot;
	FOnButtonPressed OnReadyButtonPressed;
	FOnButtonPressed OnCancelButtonPressed;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<UMapDataAsset*> MapDataAssets;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMapSelectorCell> MapSelectorCellClass;

	UPROPERTY()
	ULobbyWidget* LobbyWidget;

public:
	void UpdatePlayerList(const TArray<struct FLobbyPlayerInfo>& NewPlayersInfo, const struct FLobbyGameInfo& GameInfo);
	void PlayerCanLaunchGame(bool CanLaunch);
	void ChangeWidgetToLaunch();

protected:
	virtual void BeginPlay() override;

private:
	bool IsPlayerHost();
	void ConfigureLobbyWidget();
	void LaunchGame(FString MapName);
	void CancelGame();
	void PlayerPressedReady();

};
