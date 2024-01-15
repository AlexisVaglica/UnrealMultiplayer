// Fill out your copyright notice in the Description page of Project Settings.


#include "PyramidPlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PyramidPlayerState.h"
#include "PyramidProject/PyramidProjectHUD.h"

void APyramidPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ConfigureCurrentHUD();
}

void APyramidPlayerController::ConfigureCurrentHUD()
{
	if (CurrentHUD == nullptr)
	{
		AHUD* PlayerHUD = GetHUD();
		CurrentHUD = Cast<APyramidProjectHUD>(PlayerHUD);
	}
}

void APyramidPlayerController::ChangeScore(int BoxCount)
{
	if (HasAuthority()) 
	{
		MulticastChangeScore(BoxCount);
		ChangeScoreInHUD(BoxCount);
	}
}

void APyramidPlayerController::MulticastChangeScore_Implementation(int BoxCount)
{
	APyramidPlayerState* CurrentPlayerState = Cast<APyramidPlayerState>(PlayerState);
	CurrentPlayerState->SetFibonacciScore(BoxCount);
}

void APyramidPlayerController::ChangeScoreInHUD_Implementation(int BoxCount)
{
	float CurrentScore = PlayerState->GetScore();
	CurrentHUD->SetScorePoints(CurrentScore);
}

void APyramidPlayerController::ChangeToGameOver_Implementation(const TArray<APlayerState*>& PlayerList)
{
	CurrentHUD->SetGameOverVisibility(PlayerList);
}
