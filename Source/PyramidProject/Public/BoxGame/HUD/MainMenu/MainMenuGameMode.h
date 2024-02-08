// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;
class UMultiplayerDataAsset;
class UMapSelectorCell;
class UMultiplayerSessionSubsystem;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	int32 MaxGamesSearchCount{ 200 };

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> LobbyMap;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<UMultiplayerDataAsset*> MultiplayerMapData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMapSelectorCell> MapSelectorCellClass;

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY(EditAnywhere)
	UMultiplayerSessionSubsystem* MultiplayerSession;

protected:
	virtual void BeginPlay() override;

private:
	void ConfigureMainMenuWidget();
	void ConfigureOnlineSubsystem();
	void LaunchHostGame();
	void LaunchSoloGame(FString MapName);
	void RefreshGameList();
	void QuitGame();

	void ShowErrorMessage(FString ErrorMessage);

	UFUNCTION()
	void CreateSessionComplete(bool bWasSuccessful);
};
