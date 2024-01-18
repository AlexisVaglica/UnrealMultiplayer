// Fill out your copyright notice in the Description page of Project Settings.


#include "PyramidPlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "PyramidPlayerController.h"

void APyramidPlayerState::SetFibonacciScore(float ScoreDelta)
{
	int TermA = 0;
	int TermB = 1;
	for (int TermIdx = 0; TermIdx < ScoreDelta; TermIdx++)
	{
		int NextTerm = TermA + TermB;
		TermA = TermB;
		TermB = NextTerm;
	}

	float Result = GetScore() + TermB;
	SetScore(Result);
}

void APyramidPlayerState::OnRep_Score()
{
	Super::OnRep_Score();

	FString RepPlayerName = GetPlayerName();
	float RepScore = GetScore();

	APlayerController* LocalPlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (LocalPlayerController && LocalPlayerController->IsLocalController())
	{
		APyramidPlayerController* PyramidPlayerController = Cast<APyramidPlayerController>(LocalPlayerController);
		
		if (PyramidPlayerController) 
		{
			PyramidPlayerController->UpdateScoreboard(RepPlayerName, RepScore);
		}
	}
}
