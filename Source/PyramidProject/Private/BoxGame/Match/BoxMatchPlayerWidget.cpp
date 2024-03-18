// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Match/BoxMatchPlayerWidget.h"
#include "BoxGame/Match/Score/ScoreCellWidget.h"
#include "BoxGame/Match/Score/ScoreCellWidget.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/GameState.h"

void UBoxMatchPlayerWidget::ConfigureWidget(bool IsPlayerAuthority)
{
	if (IsPlayerAuthority)
	{
		ResetButton->OnClicked.AddDynamic(this, &ThisClass::ResetButtonPressed);
		BackMainMenuButton->OnClicked.AddDynamic(this, &ThisClass::BackMainMenuButtonPressed);
	}

	NotifyText->SetVisibility(ESlateVisibility::Hidden);
	GameoverText->SetVisibility(ESlateVisibility::Hidden);
	VBButtons->SetVisibility(ESlateVisibility::Hidden);
	ShootBarImage->SetVisibility(ESlateVisibility::Hidden);
}

void UBoxMatchPlayerWidget::SetNotify(bool IsVisible, FString NotifyString)
{
	ESlateVisibility VisibilityType = IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	NotifyText->SetVisibility(VisibilityType);
	FText NewNotify = FText::FromString(NotifyString);
	NotifyText->SetText(NewNotify);
}

void UBoxMatchPlayerWidget::DisplayPlayerName(FString PlayerName)
{
	if (PlayerNameText)
	{
		FText NewPlayerNameText = FText::FromString(PlayerName);
		PlayerNameText->SetText(NewPlayerNameText);
	}
}

void UBoxMatchPlayerWidget::SetScorePoints(float ScorePoint, float ScoreAdded, bool ShowScoreAdded)
{
	if (PlayerScoreText)
	{
		FString ScoreString = FString::Printf(TEXT("Score: %d"), (int)ScorePoint);
		FText ScoreText = FText::FromString(ScoreString);
		PlayerScoreText->SetText(ScoreText);
	}

	if (TextNewScore && NewScoreAnim)
	{
		ESlateVisibility NewScoreTextVisibility = ShowScoreAdded ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		TextNewScore->SetVisibility(NewScoreTextVisibility);

		FString ScoreString = FString::Printf(TEXT("+%d"), (int)ScoreAdded);
		FText ScoreText = FText::FromString(ScoreString);
		TextNewScore->SetText(ScoreText);

		PlayAnimation(NewScoreAnim);
	}
}

void UBoxMatchPlayerWidget::SetGameOverVisibility(const TArray<FPlayerWidgetInfo>& PlayerList, bool IsPlayerAuthority, TSubclassOf<UScoreCellWidget> ScoreCellWidgetClass)
{
	if (GameoverText)
	{
		GameoverText->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsPlayerAuthority)
	{
		VBButtons->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetNotify(true, WaitNotify);
	}

	for (FPlayerWidgetInfo CurrentPlayerName : PlayerList)
	{
		FString StringPlayerName = CurrentPlayerName.PlayerName;
		float Score = CurrentPlayerName.Score;
		CreateScoreboardCell(StringPlayerName, Score, ScoreCellWidgetClass);
	}

	if (WinPlayerNameText)
	{
		WinPlayerNameText->SetVisibility(ESlateVisibility::Visible);
		FString PlayerName = PlayerList[0].PlayerName;
		FString WinPlayerString = FString::Printf(TEXT("%s Win"), *PlayerName);
		FText WinPlayerText = FText::FromString(WinPlayerString);
		WinPlayerNameText->SetText(WinPlayerText);
	}
}

void UBoxMatchPlayerWidget::CreateScoreboardCell(FString PlayerName, int ScorePoints, TSubclassOf<UScoreCellWidget> ScoreCellWidgetClass)
{
	if (!VBScoreboard)
	{
		return;
	}

	UScoreCellWidget* NewScoreCell = CreateWidget<UScoreCellWidget>(GetWorld(), ScoreCellWidgetClass);

	ScoreCells.Add(PlayerName, NewScoreCell);

	VBScoreboard->AddChildToVerticalBox(NewScoreCell);

	ConfigureScoreCell(NewScoreCell, PlayerName, ScorePoints);
}

void UBoxMatchPlayerWidget::UpdatePlayerScore(FString PlayerName, float Score)
{
	if (ScoreCells.Num() > 0)
	{
		UScoreCellWidget* CurrentCell = ScoreCells[PlayerName];
		ConfigureScoreCell(CurrentCell, PlayerName, Score);
	}
}

void UBoxMatchPlayerWidget::ConfigureCountdownMatch(float CountdownTime)
{
	if (StartMatchText) 
	{
		int32 Seconds = FMath::FloorToInt(CountdownTime);
		FString CountdownString = FString::Printf(TEXT("The Match Start in: %02d"), Seconds);
		FText CountdownText = FText::FromString(CountdownString);
		StartMatchText->SetText(CountdownText);

		ESlateVisibility TextVisibility = CountdownTime > 0 ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		StartMatchText->SetVisibility(TextVisibility);
	}
}

void UBoxMatchPlayerWidget::StartShootBar(float Time)
{
	ShootBarImage->SetVisibility(ESlateVisibility::Visible);

	if (ShootBarMaterial == nullptr)
	{
		ShootBarMaterial = ShootBarImage->GetDynamicMaterial();
	}

	ShootBarMaterial->SetScalarParameterValue(FName(DecimalParamName), 0.f);

	MaxTimeToReloadBar = Time;
	CurrentTimeToReload = 0.f;
	ShootBarDecimal = 0.f;
	bShootBarStarting = true;
}

void UBoxMatchPlayerWidget::StopShootBar()
{
	ShootBarImage->SetVisibility(ESlateVisibility::Hidden);
	bShootBarStarting = false;
}

void UBoxMatchPlayerWidget::ShootBarTick(float TimeDelta)
{
	if (bShootBarStarting && ShootBarDecimal < ShootBarMaxDecimal && ShootBarMaterial)
	{
		CurrentTimeToReload += TimeDelta;
		ShootBarDecimal = ShootBarMaxDecimal * (CurrentTimeToReload / MaxTimeToReloadBar);
		ShootBarMaterial->SetScalarParameterValue(FName(DecimalParamName), ShootBarDecimal);
	}
}

void UBoxMatchPlayerWidget::ConfigureScoreCell(UScoreCellWidget* ScoreCell, FString& PlayerName, int ScorePoints)
{
	if (ScoreCell != nullptr)
	{
		ScoreCell->SetupCell(PlayerName, ScorePoints);
	}
}

void UBoxMatchPlayerWidget::ResetButtonPressed()
{
	VBButtons->SetVisibility(ESlateVisibility::Hidden);
	SetNotify(true, RestartingNotify);
	ResetOnButtonPressed.ExecuteIfBound();
}

void UBoxMatchPlayerWidget::BackMainMenuButtonPressed()
{
	VBButtons->SetVisibility(ESlateVisibility::Hidden);
	SetNotify(true, BackToMainMenuNotify);
	BackMainMenuOnButtonPressed.ExecuteIfBound();
}
