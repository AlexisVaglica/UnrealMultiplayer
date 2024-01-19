// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxGame/Controllers/PyramidProjectGameMode.h"
#include "BoxGame/HUD/PyramidProjectHUD.h"
#include "BoxGame/Character/PyramidProjectCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "BoxGame/Manager/PyramidManager.h"
#include "BoxGame/Controllers/PyramidPlayerState.h"
#include "BoxGame/Controllers/PyramidPlayerController.h"

APyramidProjectGameMode::APyramidProjectGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	HUDClass = APyramidProjectHUD::StaticClass();
	PlayerStateClass = APyramidPlayerState::StaticClass();
	PlayerControllerClass = APyramidPlayerController::StaticClass();
}

void APyramidProjectGameMode::BeginPlay() {

	Super::BeginPlay();

	PyramidManager = GetWorld()->SpawnActor<APyramidManager>(PyramidSpawnerClass, FVector(0, 0, 0), FRotator(0, 0, 0));
	PyramidManager->OnPyramidChange.BindUObject(this, &APyramidProjectGameMode::ChangePlayerScore);
	PyramidManager->OnPyramidDestroyed.BindUObject(this, &APyramidProjectGameMode::GameOver);
}

void APyramidProjectGameMode::ChangePlayerScore(int32 BoxCount, AActor* DamagePlayer)
{
	APyramidProjectCharacter* PyramidCharacter = Cast<APyramidProjectCharacter>(DamagePlayer);

	if (PyramidCharacter) 
	{
		AController* PlayerController = PyramidCharacter->GetPlayerController();
		if (PlayerController) 
		{
			APyramidPlayerController* PyramidPlayerController = Cast<APyramidPlayerController>(PlayerController);
			if (PyramidPlayerController)
			{
				PyramidPlayerController->ChangeScore(BoxCount);
			}
		}
	}
}

void APyramidProjectGameMode::GameOver()
{
	TArray<APlayerState*> PlayerStateList;

	for (APlayerController* PlayerController : PlayerList)
	{
		PlayerStateList.Add(PlayerController->PlayerState);
	}

	PlayerStateList.Sort([] (const APlayerState& a, const APlayerState& b) { return a.GetScore() > b.GetScore(); });

	for (APlayerController* &PlayerController : PlayerList)
	{
		APyramidPlayerController* PyramidPlayerController = Cast<APyramidPlayerController>(PlayerController);

		if (PyramidPlayerController)
		{
			PyramidPlayerController->ChangeToGameOver(PlayerStateList);
		}
	}
}

void APyramidProjectGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APyramidPlayerState* PlayerState = NewPlayer->GetPlayerState<APyramidPlayerState>();

	if (PlayerState) 
	{
		FString NewName = "Player ";
		int PlayerNum = PlayerList.Num() + 1;
		NewName.Append(FString::FromInt(PlayerNum));

		PlayerState->SetPlayerName(NewName);
	}

	PlayerList.Add(NewPlayer);
}

AActor* APyramidProjectGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	FString PlayerTag = "Player";
	PlayerTag.Append(FString::FromInt(PlayerStartIndex));
	PlayerStartIndex++;
	return FindPlayerStart(Player, PlayerTag);
}

void APyramidProjectGameMode::RestartGameplay() 
{
	RestartGame();
	UE_LOG(LogTemp, Warning, TEXT("RestartGameplay"));
}