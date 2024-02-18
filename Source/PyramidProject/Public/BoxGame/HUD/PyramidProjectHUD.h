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
class UImage;
class UMaterialInstanceDynamic;

UCLASS()
class APyramidProjectHUD : public AHUD
{
	GENERATED_BODY()

private:
	UTextBlock* DisplayText;
	UTextBlock* GameoverText;
	UTextBlock* WinPlayerNameText;
	UTextBlock* PlayerNameText;
	UTextBlock* NotifyText;
	UButton* ResetButton;
	UButton* BackMainMenuButton;
	UVerticalBox* VBButtons;
	UVerticalBox* VBScoreboard;
	UImage* ShootBarImage;

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
	const FName TextWinPlayerName{ FName(TEXT("TxtBox_WinPlayer")) };
	const FName ButtonResetName{ FName(TEXT("Btn_Reset")) };
	const FName ButtonMainMenuName{ FName(TEXT("Btn_Menu")) };
	const FName VerticalBoxName{ FName(TEXT("VB_Buttons")) };
	const FName ScoreboardName{ FName(TEXT("VB_Player_List")) };
	const FName ShootBarName{ FName(TEXT("Img_ShootBar")) };

	const FString WaitNotify{ TEXT("Waiting for the Host...") };
	const FString RestartingNotify{ TEXT("Restarting...") };
	const FString BackToMainMenuNotify{ TEXT("Backing Back To Main Menu...") };

	UMaterialInstanceDynamic* ShootBarMaterial;

	bool bShootBarStarting{ false };
	float ShootBarDecimal{ 0.f };
	float ShootBarMaxDecimal{ 1.f };
	float MaxTimeToReloadBar{ 0.f };
	float CurrentTimeToReload{ 0.f };
	const FString DecimalParamName{ TEXT("Decimal") };

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

	void StartShootBar(float Time);
	void StopShootBar();

protected:
	virtual void Tick(float TimeDelta) override;

private:
	void CreateScoreboardCell(FString PlayerName, int ScorePoints);
	void ConfigureScoreCell(UScoreCellWidget* ScoreCell, FString& PlayerName, int ScorePoints);
	void ConfigureWidget();

	void SetNotify(bool IsVisible, FString NotifyString);

	bool IsPlayerAuthority();

	UFUNCTION(Server, Reliable)
	void ResetButtonPressed();

	UFUNCTION(Server, Reliable)
	void BackMainMenuButtonPressed();
};

