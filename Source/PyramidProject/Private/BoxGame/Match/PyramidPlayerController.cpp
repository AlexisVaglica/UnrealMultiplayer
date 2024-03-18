 // Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Match/PyramidPlayerController.h"
#include "BoxGame/Match/PyramidPlayerState.h"
#include "BoxGame/Match/PyramidProjectHUD.h"
#include "BoxGame/Match/PyramidProjectCharacter.h"
#include "Net/UnrealNetwork.h"

void APyramidPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ConfigureCharacter();
	ConfigureCurrentHUD();
}

void APyramidPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMatchStarted) 
	{
		CountdownTimer();
		CheckTimeSync(DeltaTime);
	}
}

void APyramidPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();

	if (IsLocalController()) 
	{
		float TimeOfClientRequest = GetWorld()->GetTimeSeconds();
		ServerRequestServerTime(TimeOfClientRequest);
	}
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

	if (PyramidCharacter) 
	{
		PyramidCharacter->OnCharacterStartFire.BindUObject(this, &ThisClass::CharacterStartFire);
		PyramidCharacter->OnCharacterFinishFire.BindUObject(this, &ThisClass::CharacterFinishFire);

		DisableCharacter();
	}
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
		ClientChangeScoreInHUD(BoxCount);
	}
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

void APyramidPlayerController::CountdownTimer()
{
	CurrentCountdownTime = MaxCountdownTime - GetServerTime();

	if (CurrentHUD) 
	{
		CurrentHUD->ConfigureCountdownTimer(CurrentCountdownTime);
	}
}

void APyramidPlayerController::CheckTimeSync(float DeltaTime)
{
	TimeSyncRunningTime += DeltaTime;

	if (IsLocalController() && TimeSyncRunningTime > TimeSyncFrecuency) 
	{
		float TimeOfClientRequest = GetWorld()->GetTimeSeconds();
		ServerRequestServerTime(TimeOfClientRequest);
		TimeSyncRunningTime = 0.f;
	}
}

float APyramidPlayerController::GetServerTime()
{
	if (HasAuthority()) 
	{
		return GetWorld()->GetTimeSeconds();
	}
	else
	{
		return GetWorld()->GetTimeSeconds() + ClientServerDeltaTime;
	}
}

void APyramidPlayerController::MulticastChangeScore_Implementation(int BoxCount)
{
	APyramidPlayerState* CurrentPlayerState = Cast<APyramidPlayerState>(PlayerState);
	CurrentPlayerState->SetFibonacciScore(BoxCount);
}

void APyramidPlayerController::ClientChangeScoreInHUD_Implementation(int BoxCount)
{
	APyramidPlayerState* CurrentPlayerState = Cast<APyramidPlayerState>(PlayerState);

	if (CurrentPlayerState)
	{
		float CurrentScore = CurrentPlayerState->GetScore();
		float GetAddedScore = CurrentPlayerState->GetAddedScore((float)BoxCount);
		CurrentHUD->SetScorePoints(CurrentScore, GetAddedScore);
	}
}

void APyramidPlayerController::ClientChangeToGameOver_Implementation(const TArray<APlayerState*>& PlayerList)
{
	DisableCharacter();
	SetShowMouseCursor(true);

	CurrentHUD->SetGameOverVisibility(PlayerList);
}

void APyramidPlayerController::ClientStartMatchController_Implementation()
{
	bIsMatchStarted = true;

	if (PyramidCharacter)
	{
		PyramidCharacter->EnableCharacter();
	}
}

void APyramidPlayerController::ClientUpdateStartMatch_Implementation(bool IsMatchStarted, float NewMaxCountdownTime)
{
	bIsMatchStarted = IsMatchStarted;
	MaxCountdownTime = NewMaxCountdownTime;
	CurrentCountdownTime = MaxCountdownTime;
}

void APyramidPlayerController::ServerRequestServerTime_Implementation(float TimeOfClientRequest)
{
	float ServerTimeOfReceived = GetWorld()->GetTimeSeconds();
	ClientRequestServerTime(TimeOfClientRequest, ServerTimeOfReceived);
}

void APyramidPlayerController::ClientRequestServerTime_Implementation(float TimeOfClientRequest, float TimeServerReceivedClientRequest)
{
	float RoundTripTime = GetWorld()->GetTimeSeconds() - TimeOfClientRequest;
	float CurrentServerTime = TimeServerReceivedClientRequest + (RoundTripTime / 2);

	ClientServerDeltaTime = CurrentServerTime - GetWorld()->GetTimeSeconds();
}

