// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

class ULobbyWidget;
class UMultiplayerDataAsset;
class UMapSelectorCell;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULobbyWidget> LobbyWidgetClass;

	UPROPERTY(EditAnywhere)
	TArray<UMultiplayerDataAsset*> MultiplayerMapData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMapSelectorCell> MapSelectorCellClass;

	UPROPERTY()
	ULobbyWidget* LobbyWidget;

protected:
	virtual void BeginPlay() override;

private:
	void ConfigureLobbyWidget();
	void LaunchGame(FString MapName);
	void QuitGame();
};
