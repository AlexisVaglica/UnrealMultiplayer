// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "PyramidProjectGameMode.generated.h"

class APyramidProjectCharacter;
class APyramidManager;

UCLASS(minimalapi)
class APyramidProjectGameMode : public AGameMode
{
	GENERATED_BODY()

	//Override begin play from the base class
	virtual void BeginPlay() override; 

public:
	APyramidProjectGameMode();

protected:
	int PlayerStartIndex = 1;
	TArray<class APlayerController*> PlayerList;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<APyramidManager> PyramidSpawnerClass;

	UPROPERTY(EditDefaultsOnly)
	APyramidManager* MyPyramidSpawner;

protected:
	void ChangePlayerScore(int32 BoxCount, AActor* DamagePlayer);
	void GameOver();
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual void PostLogin(APlayerController* NewPlayer) override;
};



