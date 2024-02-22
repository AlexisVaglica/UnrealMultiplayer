// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LobbyPlayerController.generated.h"

class UMapDataAsset;
class ALobbyHUD;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY(Replicated)
	bool bIsReady;

	UPROPERTY()
	ALobbyHUD* LobbyHUD;

public:
	UFUNCTION(Client, Reliable)
	void ClientUpdatePlayerList(const TArray<struct FLobbyPlayerInfo>& PlayerInfoArray, const struct FLobbyGameInfo& GameInfo);

	UFUNCTION(Client, Unreliable)
	void ClientChangeWidgetToLaunch();

	UFUNCTION(Client, Unreliable)
	void ClientChangeMapSelected(const FString& MapName);

	FORCEINLINE bool IsPlayerReady() { return bIsReady; };

protected:
	virtual void BeginPlay() override;
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const;

private:
	void LaunchGame();
	void CancelGame();
	void SetIsReadyState(bool NewReadyState);
	void PlayerChangeReady();

	bool IsPlayerHost() const;
	bool CanGameStart() const;

	void RequestServerReplicateMapSelected(FString MapName);
	void RequestServerPlayerListUpdate();

	UFUNCTION(Server, Reliable)
	void ServerRequestPlayerListUpdate();

	UFUNCTION(Server, Reliable)
	void ServerRequestMapUpdate();

	UFUNCTION(Server, Reliable)
	void ServerSetIsReadyState(bool NewReadyState);
};
