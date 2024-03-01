// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BoxMatchPlayerWidget.generated.h"

class UUserWidget;
class UButton;
class UVerticalBox;
class UImage;
class UMaterialInstanceDynamic;
class UWidgetAnimation;
class UTextBlock;
class UScoreCellWidget;

DECLARE_DELEGATE(FOnButtonPressed);

USTRUCT()
struct FPlayerWidgetInfo 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	FString PlayerName;

	UPROPERTY()
	int32 Score;
};

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UBoxMatchPlayerWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnButtonPressed ResetOnButtonPressed;
	FOnButtonPressed BackMainMenuOnButtonPressed;

private:
	UPROPERTY(meta=(BindWidgetOptional))
	UTextBlock* PlayerScoreText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* GameoverText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* WinPlayerNameText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* PlayerNameText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* NotifyText;

	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextNewScore;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* ResetButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BackMainMenuButton;

	UPROPERTY(meta = (BindWidgetOptional))
	UVerticalBox* VBButtons;

	UPROPERTY(meta = (BindWidgetOptional))
	UVerticalBox* VBScoreboard;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ShootBarImage;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* NewScoreAnim;

	UPROPERTY()
	UMaterialInstanceDynamic* ShootBarMaterial;

	TMap<FString, UScoreCellWidget*> ScoreCells;

	const FString WaitNotify{ TEXT("Waiting for the Host...") };
	const FString RestartingNotify{ TEXT("Restarting...") };
	const FString BackToMainMenuNotify{ TEXT("Backing Back To Main Menu...") };

	bool bShootBarStarting{ false };
	float ShootBarDecimal{ 0.f };
	float ShootBarMaxDecimal{ 1.f };
	float MaxTimeToReloadBar{ 0.f };
	float CurrentTimeToReload{ 0.f };
	const FString DecimalParamName{ TEXT("Decimal") };

public:
	void ConfigureWidget(bool IsPlayerAuthority);
	void SetScorePoints(float ScorePoint, float ScoreAdded, bool ShowScoreAdded = true);
	void SetGameOverVisibility(const TArray<FPlayerWidgetInfo>& PlayerList, bool IsPlayerAuthority, TSubclassOf<UScoreCellWidget> ScoreCellWidgetClass);
	void DisplayPlayerName(FString PlayerName);
	void UpdatePlayerScore(FString PlayerName, float Score);

	void StartShootBar(float Time);
	void StopShootBar();

	void ShootBarTick(float TimeDelta);

private:
	void CreateScoreboardCell(FString PlayerName, int ScorePoints, TSubclassOf<UScoreCellWidget> ScoreCellWidgetClass);
	void ConfigureScoreCell(UScoreCellWidget* ScoreCell, FString& PlayerName, int ScorePoints);

	void SetNotify(bool IsVisible, FString NotifyString);

	UFUNCTION()
	void ResetButtonPressed();

	UFUNCTION()
	void BackMainMenuButtonPressed();
};
