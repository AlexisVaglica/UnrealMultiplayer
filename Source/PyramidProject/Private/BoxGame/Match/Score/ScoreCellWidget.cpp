// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Match/Score/ScoreCellWidget.h"
#include "Components/TextBlock.h"

void UScoreCellWidget::SetupCell(FString PlayerName, float Score)
{
	FString ScoreString = PlayerName;
	ScoreString.Append(" - Score: ");
	ScoreString.Append(FString::FromInt((int)Score));
	FText NewScoreText = FText::FromString(ScoreString);
	Txt_PlayerScore->SetText(NewScoreText);
}
