// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxGame/Match/PyramidProjectGameMode.h"
#include "BoxGame/Match/PyramidProjectHUD.h"
#include "BoxGame/Match/PyramidProjectCharacter.h"
#include "BoxGame/Match/Pyramid/PyramidManager.h"
#include "BoxGame/Match/PyramidPlayerState.h"
#include "BoxGame/Match/PyramidPlayerController.h"
#include "BoxGame/Utils/ReadWriteJsonFile.h"
#include "BoxGame/Utils/PlayerSettings.h"
#include "MultiplayerSession/Public/Multiplayer/MultiplayerSessionSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

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

	CurrentCountdownTime = MaxCountdownTime;

	ConfigurePyramidManager();
	ConfigureOnlineSubsystem();
}

void APyramidProjectGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsMatchStarted) 
	{
		CountdownToStartMatch(DeltaTime);
	}
}

void APyramidProjectGameMode::ConfigurePlayerSettings()
{
	PlayerSettings = NewObject<UPlayerSettings>();

	if (PlayerSettings)
	{
		bool bSuccess = false;
		FString Message;

		UReadWriteJsonFile::ReadJson(PlayerSettings->PlayerSettingsPath, bSuccess, Message, PlayerSettings);
	}
}

void APyramidProjectGameMode::ConfigureOnlineSubsystem()
{
	UGameInstance* GameInstance = GetGameInstance();

	if (GameInstance)
	{
		MultiplayerSession = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();

		if (MultiplayerSession)
		{
			MultiplayerSession->MultiplayerOnDestroySessionComplete.AddDynamic(this, &ThisClass::DestroySessionComplete);
		}
	}
}

void APyramidProjectGameMode::ConfigurePyramidManager()
{
	PyramidManager = GetWorld()->SpawnActor<APyramidManager>(PyramidSpawnerClass, FVector(0, 0, 0), FRotator(0, 0, 0));

	if (PyramidManager)
	{
		PyramidManager->OnPyramidChange.BindUObject(this, &APyramidProjectGameMode::ChangePlayerScore);
		PyramidManager->OnPyramidDestroyed.BindUObject(this, &APyramidProjectGameMode::GameOver);
	}
}

void APyramidProjectGameMode::CountdownToStartMatch(float DeltaTime)
{
	if (CurrentCountdownTime <= 0) 
	{
		bIsMatchStarted = true;
		PlayerStartMatch();
	}
	else
	{
		CurrentCountdownTime = MaxCountdownTime - GetWorld()->GetTimeSeconds();
	}
}

void APyramidProjectGameMode::PlayerStartMatch()
{
	for (APlayerController* PlayerController : PlayerList)
	{
		APyramidPlayerController* PyramidPlayerController = Cast<APyramidPlayerController>(PlayerController);
		if (PyramidPlayerController)
		{
			PyramidPlayerController->ClientStartMatchController();
		}
	}
}

void APyramidProjectGameMode::DestroySessionComplete(bool bWasSuccessful)
{
	FString MainMenuMapPath = FString::Printf(TEXT("%s?closed"), *MainMenuMap.GetLongPackageName());
	UGameplayStatics::OpenLevel(GetWorld(), FName(MainMenuMapPath), true);
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
			PyramidPlayerController->ClientChangeToGameOver(PlayerStateList);
		}
	}
}

void APyramidProjectGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);

	APyramidPlayerState* PlayerState = NewPlayer->GetPlayerState<APyramidPlayerState>();

	APyramidPlayerController* PyramidPlayerController = Cast<APyramidPlayerController>(NewPlayer);
	if (PyramidPlayerController) 
	{
		PyramidPlayerController->ClientUpdateStartMatch(bIsMatchStarted, MaxCountdownTime);
	}

	ConfigurePlayerSettings();

	if (PlayerState && PlayerSettings)
	{
		FString NewName = PlayerSettings->LocalPlayerInfo.LocalName;
		PlayerState->SetPlayerName(NewName);
	}

	PlayerList.Add(NewPlayer);
}

void APyramidProjectGameMode::RestartGameplay() 
{
	RestartGame();
}

void APyramidProjectGameMode::BackMainMenu()
{
	if (MultiplayerSession)
	{
		MultiplayerSession->DestroySession();
	}
}

AActor* APyramidProjectGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	FString PlayerTag = "Player";
	PlayerTag.Append(FString::FromInt(PlayerStartIndex));
	PlayerStartIndex++;
	return FindPlayerStart(Player, PlayerTag);
}

