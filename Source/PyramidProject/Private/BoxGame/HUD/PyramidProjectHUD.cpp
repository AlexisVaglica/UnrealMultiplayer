// Copyright Epic Games, Inc. All Rights Reserved.

#include "BoxGame/HUD/PyramidProjectHUD.h"
#include "BoxGame/Controllers/PyramidPlayerState.h"
#include "BoxGame/Controllers/PyramidProjectGameMode.h"
#include "BoxGame/HUD/Score/ScoreCellWidget.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "GameFramework/GameState.h"
#include "Kismet/GameplayStatics.h"
#include "CanvasItem.h"
#include "TextureResource.h"

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
		UserWidget = CreateWidget<UUserWidget>(GetWorld(), UserWidgetClass);

		if (UserWidget)
		{
			UserWidget->AddToViewport();

			DisplayText = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TextScoreName));
			GameoverText = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TextGameoverName));
			PlayerNameText = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TextPlayerName));
			NotifyText = Cast<UTextBlock>(UserWidget->GetWidgetFromName(TextNotifyName));
			ResetButton = Cast<UButton>(UserWidget->GetWidgetFromName(ButtonResetName));
			BackMainMenuButton = Cast<UButton>(UserWidget->GetWidgetFromName(ButtonMainMenuName));

			ShootBarImage = Cast<UImage>(UserWidget->GetWidgetFromName(ShootBarName));

			VBScoreboard = Cast<UVerticalBox>(UserWidget->GetWidgetFromName(ScoreboardName));
			VBButtons = Cast<UVerticalBox>(UserWidget->GetWidgetFromName(VerticalBoxName));

			NotifyText->SetVisibility(ESlateVisibility::Hidden);
			GameoverText->SetVisibility(ESlateVisibility::Hidden);
			VBButtons->SetVisibility(ESlateVisibility::Hidden);
			ShootBarImage->SetVisibility(ESlateVisibility::Hidden);

			if (IsPlayerAuthority())
			{
				ResetButton->OnClicked.AddDynamic(this, &ThisClass::ResetButtonPressed);
				BackMainMenuButton->OnClicked.AddDynamic(this, &ThisClass::BackMainMenuButtonPressed);
			}

			SetScorePoints(0);
		}
	}
}

void APyramidProjectHUD::SetNotify(bool IsVisible, FString NotifyString)
{
	ESlateVisibility VisibilityType = IsVisible ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	NotifyText->SetVisibility(VisibilityType);
	FText NewNotify = FText::FromString(NotifyString);
	NotifyText->SetText(NewNotify);
}

bool APyramidProjectHUD::IsPlayerAuthority()
{
	return GetNetMode() == ENetMode::NM_ListenServer || GetNetMode() == ENetMode::NM_Standalone;
}

void APyramidProjectHUD::DisplayPlayerName()
{
	auto CurrentPS = GetOwningPlayerController()->GetPlayerState<APyramidPlayerState>();
	if (CurrentPS)
	{
		if (PlayerNameText)
		{
			FString DisplayName = CurrentPS->GetPlayerName();
			FText NewPlayerNameText = FText::FromString(DisplayName);
			PlayerNameText->SetText(NewPlayerNameText);
		}
	}
}

void APyramidProjectHUD::SetScorePoints(float ScorePoint) 
{
	if (DisplayText) 
	{
		FString ScoreString = "Score: ";
		ScoreString.Append(FString::FromInt((int)ScorePoint));
		FText ScoreText = FText::FromString(ScoreString);
		DisplayText->SetText(ScoreText);
	}
}

void APyramidProjectHUD::SetGameOverVisibility(const TArray<APlayerState*>& PlayerList)
{
	if (GameoverText) 
	{
		GameoverText->SetVisibility(ESlateVisibility::Visible);
	}

	if (IsPlayerAuthority()) 
	{
		VBButtons->SetVisibility(ESlateVisibility::Visible);
	}
	else
	{
		SetNotify(true, WaitNotify);
	}

	for(APlayerState* PlayerState : PlayerList)
	{
		FString PlayerName = PlayerState->GetPlayerName();
		float Score = PlayerState->GetScore();
		CreateScoreboardCell(PlayerName, Score);
	}
}

void APyramidProjectHUD::CreateScoreboardCell(FString PlayerName, int ScorePoints)
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

void APyramidProjectHUD::UpdatePlayerScore(FString PlayerName, float Score)
{
	if (ScoreCells.Num() > 0)
	{
		UScoreCellWidget* CurrentCell = ScoreCells[PlayerName];
		ConfigureScoreCell(CurrentCell, PlayerName, Score);
	}
}

void APyramidProjectHUD::StartShootBar(float Time)
{

	ShootBarImage->SetVisibility(ESlateVisibility::Visible);

	if(ShootBarMaterial == nullptr)
	{
		ShootBarMaterial = ShootBarImage->GetDynamicMaterial();
	}

	ShootBarMaterial->SetScalarParameterValue(FName(DecimalParamName), 0.f);

	MaxTimeToReloadBar = Time;
	CurrentTimeToReload = 0.f;
	ShootBarDecimal = 0.f;
	bShootBarStarting = true;
}

void APyramidProjectHUD::StopShootBar()
{
	ShootBarImage->SetVisibility(ESlateVisibility::Hidden);
	bShootBarStarting = false;
}

void APyramidProjectHUD::Tick(float TimeDelta)
{
	if (bShootBarStarting && ShootBarDecimal < ShootBarMaxDecimal && ShootBarMaterial)
	{
		CurrentTimeToReload += TimeDelta;
		ShootBarDecimal = ShootBarMaxDecimal * (CurrentTimeToReload / MaxTimeToReloadBar);
		ShootBarMaterial->SetScalarParameterValue(FName(DecimalParamName), ShootBarDecimal);
	}
}

void APyramidProjectHUD::ConfigureScoreCell(UScoreCellWidget* ScoreCell, FString& PlayerName, int ScorePoints)
{
	if (ScoreCell != nullptr)
	{
		ScoreCell->SetupCell(PlayerName, ScorePoints);
	}
}

void APyramidProjectHUD::ResetButtonPressed_Implementation()
{
	VBButtons->SetVisibility(ESlateVisibility::Hidden);
	SetNotify(true, RestartingNotify);

	APyramidProjectGameMode* CurrentGameMode = Cast<APyramidProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode) 
	{
		CurrentGameMode->RestartGameplay();
	}
}

void APyramidProjectHUD::BackMainMenuButtonPressed_Implementation()
{
	VBButtons->SetVisibility(ESlateVisibility::Hidden);
	SetNotify(true, BackToMainMenuNotify);
	
	APyramidProjectGameMode* CurrentGameMode = Cast<APyramidProjectGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentGameMode)
	{
		CurrentGameMode->BackMainMenu();
	}
}