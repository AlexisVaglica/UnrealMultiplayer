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


	TMap<FString, UTextBlock*> ScoreCells;

	UTexture2D* CrosshairTex;

	const FName TextScoreName{ FName(TEXT("TxtBox_Score")) };
	const FName TextGameoverName{ FName(TEXT("TxtBox_GameOver")) };
	const FName TextPlayerName{ FName(TEXT("TxtBox_PlayerName")) };
	const FName TextNotifyName{ FName(TEXT("TxtBox_Notify")) };
	const FName ButtonResetName{ FName(TEXT("Btn_Reset")) };
	const FName VerticalBoxName{ FName(TEXT("VB_Buttons")) };

	const FString WaitNotify{ TEXT("Waiting for the Host...") };
	const FString RestartingNotify{ TEXT("Restarting...") };

protected:
	TSubclassOf<class UUserWidget> WidgetClass;

	UUserWidget* Widget;

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
	void ConfigureScoreCell(UTextBlock* NewScoreCell, FString& PlayerName, int ScorePoints);
	void ConfigureWidget();

	void SetNotify(bool IsVisible, FString NotifyString);

	UFUNCTION(Server, Reliable)
	void ResetButtonPressed();
};

