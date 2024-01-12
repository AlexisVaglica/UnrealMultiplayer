// Fill out your copyright notice in the Description page of Project Settings.


#include "PyramidPlayerState.h"

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