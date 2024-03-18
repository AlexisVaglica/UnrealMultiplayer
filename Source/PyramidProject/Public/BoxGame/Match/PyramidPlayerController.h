// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PyramidPlayerController.generated.h"

class APyramidProjectHUD;
class APyramidProjectCharacter;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API APyramidPlayerController : public APlayerController
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	APyramidProjectHUD* CurrentHUD;

	UPROPERTY()
	APyramidProjectCharacter* PyramidCharacter;

	UPROPERTY(EditAnywhere, Category = "Time")
	float TimeSyncFrecuency{ 2.f };
	float TimeSyncRunningTime{ 0.f };

	float MaxCountdownTime{ 0.f };
	float CurrentCountdownTime{ 0.f };
	bool bIsMatchStarted{ false };

	float ClientServerDeltaTime{ 0.f };

public:
	UFUNCTION(Client, Reliable)
	void ClientChangeToGameOver(const TArray<APlayerState*>& PlayerList);

	UFUNCTION(Client, Reliable)
	void ClientUpdateStartMatch(bool IsMatchStarted, float NewMaxCountdownTime);
	
	UFUNCTION(Client, Reliable)
	void ClientStartMatchController();

	void ChangeScore(int BoxCount);
	void UpdateScoreboard(FString PlayerName, float PlayerScore);

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void ReceivedPlayer() override;

private:
	void ConfigureCurrentHUD();
	void ConfigureCharacter();
	void DisableCharacter();

	void CharacterStartFire(float Time);
	void CharacterFinishFire();

	void CountdownTimer();
	float GetServerTime();
	void CheckTimeSync(float DeltaTime);

	UFUNCTION(Client, Reliable)
	void ClientChangeScoreInHUD(int BoxCount);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeScore(int BoxCount);

	UFUNCTION(Server, Reliable)
	void ServerRequestServerTime(float TimeOfClientRequest);

	UFUNCTION(Client, Reliable)
	void ClientRequestServerTime(float TimeOfClientRequest, float TimeServerReceivedClientRequest);
};
