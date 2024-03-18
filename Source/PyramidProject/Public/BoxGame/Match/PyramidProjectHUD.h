// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "PyramidProjectHUD.generated.h"

class APyramidPlayerState;
class UTexture2D;
class UScoreCellWidget;
class UBoxMatchPlayerWidget;

UCLASS()
class APyramidProjectHUD : public AHUD
{
	GENERATED_BODY()

private:
	UPROPERTY()
	UTexture2D* CrosshairTex;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Options")
	TSubclassOf<UScoreCellWidget> ScoreCellWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Options")
	TSubclassOf<UBoxMatchPlayerWidget> UserWidgetClass;

	UPROPERTY()
	UBoxMatchPlayerWidget* UserWidget;

public:
	APyramidProjectHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	void SetScorePoints(float ScorePoint, float ScoreAdded, bool ShowScoreAdded = true);
	void SetGameOverVisibility(const TArray<APlayerState*>& PlayerList);
	void DisplayPlayerName();
	void UpdatePlayerScore(FString PlayerName, float Score);

	void StartShootBar(float Time);
	void StopShootBar();

	void ConfigureCountdownTimer(float CountdownTime);

protected:
	virtual void Tick(float TimeDelta) override;

private:
	void ConfigureWidget();
	bool IsPlayerAuthority();

	UFUNCTION(Server, Reliable)
	void ServerResetGame();

	UFUNCTION(Server, Reliable)
	void ServerBackMainMenuGame();
};

