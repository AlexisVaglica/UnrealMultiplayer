// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Controllers/PyramidPlayerController.h"
#include "BoxGame/Controllers/PyramidPlayerState.h"
#include "BoxGame/HUD/PyramidProjectHUD.h"
#include "BoxGame/Character/PyramidProjectCharacter.h"
#include "Net/UnrealNetwork.h"

void APyramidPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ConfigureCharacter();
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

void APyramidPlayerController::ConfigureCharacter()
{
	PyramidCharacter = Cast<APyramidProjectCharacter>(GetPawn());
	PyramidCharacter->OnCharacterStartFire.BindUObject(this, &ThisClass::CharacterStartFire);
	PyramidCharacter->OnCharacterFinishFire.BindUObject(this, &ThisClass::CharacterFinishFire);
}

void APyramidPlayerController::UpdateScoreboard(FString PlayerName, float PlayerScore)
{
	if (CurrentHUD) 
	{
		CurrentHUD->UpdatePlayerScore(PlayerName, PlayerScore);
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
	DisableCharacter();
	SetShowMouseCursor(true);

	CurrentHUD->SetGameOverVisibility(PlayerList);
}

void APyramidPlayerController::DisableCharacter()
{
	if (PyramidCharacter) 
	{
		PyramidCharacter->DisableCharacter();
	}
}

void APyramidPlayerController::CharacterStartFire(float Time)
{
	if (CurrentHUD) 
	{
		CurrentHUD->StartShootBar(Time);
	}
}

void APyramidPlayerController::CharacterFinishFire()
{
	if (CurrentHUD)
	{
		CurrentHUD->StopShootBar();
	}
}

