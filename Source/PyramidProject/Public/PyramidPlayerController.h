// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PyramidPlayerController.generated.h"

class APyramidProjectHUD;

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

public:
	UFUNCTION(Client, Reliable)
	void ChangeToGameOver(const TArray<APlayerState*>& PlayerList);

	void ChangeScore(int BoxCount);

	void UpdateScoreboard(FString PlayerName, float PlayerScore);

protected:
	virtual void BeginPlay() override;

private:
	void ConfigureCurrentHUD();

	void DisableCharacter();

	UFUNCTION(Client, Reliable)
	void ChangeScoreInHUD(int BoxCount);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeScore(int BoxCount);
};
