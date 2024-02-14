// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;
class UMapDataAsset;
class UMapSelectorCell;
class USessionSearchCell;
class UMultiplayerSessionSubsystem;
class UMultiplayerDataAsset;
class FOnlineSessionSearchResult;

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
	UMultiplayerDataAsset* MultiplayerData;

	UPROPERTY(EditAnywhere)
	TArray<UMapDataAsset*> MapData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMapSelectorCell> MapSelectorCellClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<USessionSearchCell> SessionSearchCellClass;

	UPROPERTY(EditAnywhere, Category="Error Message")
	FString ErrorCreateSessionMessage{ TEXT("Error to Create a Session") };

	UPROPERTY(EditAnywhere, Category = "Error Message")
	FString ErrorFindSessionsMessage{ TEXT("Error Finding Sessions") };

	UPROPERTY(EditAnywhere, Category = "Error Message")
	FString ErrorJoinSessionsMessage{ TEXT("Error Joining Sessions") };

	UPROPERTY()
	FString JoinSessionMessage{ TEXT("Connecting to Session Host...") };

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

	UPROPERTY()
	UMultiplayerSessionSubsystem* MultiplayerSession;

protected:
	virtual void BeginPlay() override;

private:
	void ConfigureMainMenuWidget();
	void ConfigureOnlineSubsystem();
	void LaunchHostGame();
	void LaunchSoloGame(FString MapName);
	void RefreshGameList();
	void JoinSessionGame(FString SessionId);
	void QuitGame();

	void ShowErrorMessage(FString ErrorMessage);

	UFUNCTION()
	void CreateSessionComplete(bool bWasSuccessful);

	UFUNCTION()
	void FindSessionsComplete(const TArray<struct FSessionGameInfo>& SessionIdResults, bool bWasSuccess);

	UFUNCTION()
	void JoinSessionComplete(bool bWasSuccessful);
};
