// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PyramidProjectGameMode.generated.h"

class APyramidProjectCharacter;
class APyramidManager;
class UMultiplayerSessionSubsystem;

UCLASS(minimalapi)
class APyramidProjectGameMode : public AGameMode
{
	GENERATED_BODY()

	//Override begin play from the base class
	virtual void BeginPlay() override; 

public:
	APyramidProjectGameMode();
	void RestartGameplay();
	void BackMainMenu();

private:
	int PlayerStartIndex = 1;
	TArray<class APlayerController*> PlayerList;

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MainMenuMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APyramidManager> PyramidSpawnerClass;

	UPROPERTY()
	APyramidManager* PyramidManager;

	UPROPERTY()
	UMultiplayerSessionSubsystem* MultiplayerSession;

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	void ChangePlayerScore(int32 BoxCount, AActor* DamagePlayer);
	void GameOver();
	void ConfigureOnlineSubsystem();
	void ConfigurePyramidManager();

	UFUNCTION()
	void DestroySessionComplete(bool bWasSuccessful);

};



