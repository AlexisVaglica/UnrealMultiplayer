// Copyright Epic Games, Inc. All Rights Reserved.

#include "PyramidProjectHUD.h"
#include "Engine/Canvas.h"
#include "Engine/Texture2D.h"
#include "TextureResource.h"
#include "CanvasItem.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "GameFramework/GameState.h"
#include "PyramidPlayerState.h"

APyramidProjectHUD::APyramidProjectHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("/Game/FirstPerson/Textures/FirstPersonCrosshair"));
	CrosshairTex = CrosshairTexObj.Object;

	ConstructorHelpers::FClassFinder<UUserWidget> WidgetAsset(TEXT("/Game/ProjectGame/Blueprints/HUD/BP_HUD"));
	if (WidgetAsset.Succeeded())
	{
		WidgetClass = WidgetAsset.Class;
	}
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

	FCanvasTileItem TileItem( CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem( TileItem );

	DisplayPlayerName();
}

void APyramidProjectHUD::CreateScoreboardCell(FString PlayerName, int ScorePoints)
{
	const FName VerticalBoxName = FName(TEXT("VB_Player_List"));
	class UVerticalBox* VerticalBox = (UVerticalBox*)(Widget->GetWidgetFromName(VerticalBoxName));

	UTextBlock* NewScoreCell = NewObject<UTextBlock>();

	ScoreCells.Add(PlayerName, NewScoreCell);

	VerticalBox->AddChildToVerticalBox(NewScoreCell);

	ConfigureScoreCell(NewScoreCell, PlayerName, ScorePoints);
}

void APyramidProjectHUD::UpdatePlayerScore(FString PlayerName, float Score)
{
	if (ScoreCells.Num() > 0) 
	{
		UTextBlock* CurrentCell = ScoreCells[PlayerName];
		ConfigureScoreCell(CurrentCell, PlayerName, Score);
	}
}

void APyramidProjectHUD::ConfigureScoreCell(UTextBlock* ScoreCell, FString& PlayerName, int ScorePoints)
{
	if (ScoreCell != nullptr)
	{
		FString ScoreString = PlayerName;
		ScoreString.Append(" - Score: ");
		ScoreString.Append(FString::FromInt((int)ScorePoints));
		FText ScoreText = FText::FromString(ScoreString);
		ScoreCell->SetText(ScoreText);
	}
}

void APyramidProjectHUD::ConfigureWidget()
{
	if (WidgetClass)
	{
		Widget = CreateWidget<UUserWidget>(GetWorld(), WidgetClass);

		if (Widget)
		{
			Widget->AddToViewport();

			DisplayText = Cast<UTextBlock>(Widget->GetWidgetFromName(TextScoreName));
			GameoverText = Cast<UTextBlock>(Widget->GetWidgetFromName(TextGameoverName));
			PlayerNameText = Cast<UTextBlock>(Widget->GetWidgetFromName(TextPlayerName));

			if (GameoverText != nullptr)
			{
				GameoverText->SetVisibility(ESlateVisibility::Hidden);
			}

			SetScorePoints(0);
		}
	}
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
	GameoverText->SetVisibility(ESlateVisibility::Visible);

	for(APlayerState* PlayerState : PlayerList)
	{
		FString PlayerName = PlayerState->GetPlayerName();
		float Score = PlayerState->GetScore();
		CreateScoreboardCell(PlayerName, Score);
	}
}
