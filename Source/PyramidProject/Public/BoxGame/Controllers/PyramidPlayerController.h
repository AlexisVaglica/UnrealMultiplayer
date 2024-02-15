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

public:
	UFUNCTION(Client, Reliable)
	void ChangeToGameOver(const TArray<APlayerState*>& PlayerList);

	void ChangeScore(int BoxCount);

	void UpdateScoreboard(FString PlayerName, float PlayerScore);

protected:
	virtual void BeginPlay() override;

private:
	void ConfigureCurrentHUD();
	void ConfigureCharacter();
	void DisableCharacter();

	void CharacterStartFire(float Time);
	void CharacterFinishFire();

	UFUNCTION(Client, Reliable)
	void ChangeScoreInHUD(int BoxCount);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastChangeScore(int BoxCount);
};
