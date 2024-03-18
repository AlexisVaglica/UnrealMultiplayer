// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PyramidProjectGameMode.generated.h"

class APyramidProjectCharacter;
class APyramidManager;
class UMultiplayerSessionSubsystem;
class UPlayerSettings;

UCLASS(minimalapi)
class APyramidProjectGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override; 

public:
	APyramidProjectGameMode();
	void RestartGameplay();
	void BackMainMenu();

private:
	int PlayerStartIndex = 1;
	TArray<class APlayerController*> PlayerList;

	float CurrentCountdownTime{ 0.f };
	bool bIsMatchStarted{ false };

	UPROPERTY(EditAnywhere, Category = "Match Time")
	float MaxCountdownTime{ 10.f };

	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UWorld> MainMenuMap;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APyramidManager> PyramidSpawnerClass;

	UPROPERTY()
	UPlayerSettings* PlayerSettings;

	UPROPERTY()
	APyramidManager* PyramidManager;

	UPROPERTY()
	UMultiplayerSessionSubsystem* MultiplayerSession;

protected:
	virtual void Tick(float DeltaTime) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

private:
	void ChangePlayerScore(int32 BoxCount, AActor* DamagePlayer);
	void GameOver();
	void ConfigurePlayerSettings();
	void ConfigureOnlineSubsystem();
	void ConfigurePyramidManager();

	void CountdownToStartMatch(float DeltaTime);
	void PlayerStartMatch();

	UFUNCTION()
	void DestroySessionComplete(bool bWasSuccessful);

};



