// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once 

#include "CoreMinimal.h"
#include "Components/TextBlock.h"
#include "GameFramework/HUD.h"
#include "PyramidProjectHUD.generated.h"

class APyramidPlayerState;
class UTexture2D;
class UUserWidget;
class UButton;
class UVerticalBox;
class UScoreCellWidget;

UCLASS()
class APyramidProjectHUD : public AHUD
{
	GENERATED_BODY()

private:
	UTextBlock* DisplayText;
	UTextBlock* GameoverText;
	UTextBlock* PlayerNameText;
	UTextBlock* NotifyText;
	UButton* ResetButton;
	UVerticalBox* VBButtons;
	UVerticalBox* VBScoreboard;

	TMap<FString, UScoreCellWidget*> ScoreCells;

	UTexture2D* CrosshairTex;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Options")
	TSubclassOf<UScoreCellWidget> ScoreCellWidgetClass;

	UPROPERTY(EditDefaultsOnly, Category = "Widget Options")
	TSubclassOf<UUserWidget> UserWidgetClass;

	const FName TextScoreName{ FName(TEXT("TxtBox_Score")) };
	const FName TextGameoverName{ FName(TEXT("TxtBox_GameOver")) };
	const FName TextPlayerName{ FName(TEXT("TxtBox_PlayerName")) };
	const FName TextNotifyName{ FName(TEXT("TxtBox_Notify")) };
	const FName ButtonResetName{ FName(TEXT("Btn_Reset")) };
	const FName VerticalBoxName{ FName(TEXT("VB_Buttons")) };
	const FName ScoreboardName{ FName(TEXT("VB_Player_List")) };

	const FString WaitNotify{ TEXT("Waiting for the Host...") };
	const FString RestartingNotify{ TEXT("Restarting...") };

protected:
	UUserWidget* UserWidget;

public:
	APyramidProjectHUD();

	virtual void BeginPlay() override;
	virtual void DrawHUD() override;

	void SetScorePoints(float ScorePoint);
	void SetGameOverVisibility(const TArray<APlayerState*>& PlayerList);
	void DisplayPlayerName();
	void UpdatePlayerScore(FString PlayerName, float Score);

private:
	void CreateScoreboardCell(FString PlayerName, int ScorePoints);
	void ConfigureScoreCell(UScoreCellWidget* ScoreCell, FString& PlayerName, int ScorePoints);
	void ConfigureWidget();

	void SetNotify(bool IsVisible, FString NotifyString);

	UFUNCTION(Server, Reliable)
	void ResetButtonPressed();
};

