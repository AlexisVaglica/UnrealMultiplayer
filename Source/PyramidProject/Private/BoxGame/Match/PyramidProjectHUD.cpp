// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxGame/Match/PyramidProjectHUD.h"
#include "BoxGame/Match/PyramidPlayerState.h"
#include "BoxGame/Match/PyramidProjectGameMode.h"
#include "BoxGame/Match/Score/ScoreCellWidget.h"
#include "BoxGame/Match/BoxMatchPlayerWidget.h"
#include "Kismet/GameplayStatics.h"
#include "CanvasItem.h"
#include "TextureResource.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"

APyramidProjectHUD::APyramidProjectHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	SetActorTickEnabled(true);
}

void APyramidProjectHUD::BeginPlay()
{
	Super::BeginPlay();

	ConfigureWidget();
}

void APyramidProjectHUD::DrawHUD()
{
	Super::DrawHUD();

	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);

	const FVector2D CrosshairDrawPosition( (Center.X),
										   (Center.Y + 20.0f));

	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->GetResource(), FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	DisplayPlayerName();
}

void APyramidProjectHUD::ConfigureWidget()
{
	if (UserWidgetClass)
	{
		UserWidget = CreateWidget<UBoxMatchPlayerWidget>(GetWorld(), UserWidgetClass);

		if (UserWidget)
		{
			UserWidget->AddToViewport();
			UserWidget->ConfigureWidget(IsPlayerAuthority());

			if(IsPlayerAuthority()) 
			{
				UserWidget->ResetOnButtonPressed.BindUObject(this, &ThisClass::ServerResetGame);
				UserWidget->BackMainMenuOnButtonPressed.BindUObject(this, &ThisClass::ServerBackMainMenuGame);
			}

			SetScorePoints(0, 0, false);
		}
	}
}

bool APyramidProjectHUD::IsPlayerAuthority()
{
	return GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone;
}

void APyramidProjectHUD::DisplayPlayerName()
{
	APyramidPlayerState* CurrentPS = GetOwningPlayerController()->GetPlayerState<APyramidPlayerState>();

	if (UserWidget && CurrentPS)
	{
		UserWidget->DisplayPlayerName(CurrentPS->GetPlayerName());
	}
}

void APyramidProjectHUD::SetScorePoints(float ScorePoint, float ScoreAdded, bool ShowScoreAdded)
{
	if (UserWidget) 
	{
		UserWidget->SetScorePoints(ScorePoint, ScoreAdded, ShowScoreAdded);
	}
}

void APyramidProjectHUD::SetGameOverVisibility(const TArray<APlayerState*>& PlayerList)
{
	if (UserWidget) 
	{
		TArray<FPlayerWidgetInfo> NewPlayerList;

		for (APlayerState* Player : PlayerList) 
		{
			FPlayerWidgetInfo NewPlayerWidgetInfo;
			NewPlayerWidgetInfo.PlayerName = Player->GetPlayerName();
			NewPlayerWidgetInfo.Score = Player->GetScore();
			NewPlayerList.Add(NewPlayerWidgetInfo);
		}

		UserWidget->SetGameOverVisibility(NewPlayerList, IsPlayerAuthority(), ScoreCellWidgetClass);
	}
}

void APyramidProjectHUD::UpdatePlayerScore(FString PlayerName, float Score)
{
	if (UserWidget) 
	{
		UserWidget->UpdatePlayerScore(PlayerName, Score);
	}
}

void APyramidProjectHUD::StartShootBar(float Time)
{
	if (UserWidget)
	{
		UserWidget->StartShootBar(Time);
	}
}

void APyramidProjectHUD::StopShootBar()
{
	if (UserWidget)
	{
		UserWidget->StopShootBar();
	}
}

void APyramidProjectHUD::ConfigureCountdownTimer(float CountdownTime)
{
	if (UserWidget) 
	{
		UserWidget->ConfigureCountdownMatch(CountdownTime);
	}
}

void APyramidProjectHUD::Tick(float TimeDelta)
{
	if (UserWidget)
	{
		UserWidget->ShootBarTick(TimeDelta);
	}
}

void APyramidProjectHUD::ServerResetGame_Implementation()
{
	APyramidProjectGameMode* CurrentGameMode = Cast<APyramidProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode) 
	{
		CurrentGameMode->RestartGameplay();
	}
}

void APyramidProjectHUD::ServerBackMainMenuGame_Implementation()
{
	APyramidProjectGameMode* CurrentGameMode = Cast<APyramidProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode)
	{
		CurrentGameMode->BackMainMenu();
	}
}