// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/MainMenuWidget.h"
#include "BoxGame/HUD/MainMenu/MapSelectorCell.h"
#include "Components/Button.h"
#include "Components/Border.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"
#include "Components/CanvasPanelSlot.h"


bool UMainMenuWidget::Initialize()
{
	if (!Super::Initialize())
	{
		return false;
	}

	if (BtnSoloGame)
	{
		BtnSoloGame->OnClicked.AddDynamic(this, &ThisClass::SoloGameBtnClicked);
	}

	if (BtnMultiplayer)
	{
		BtnMultiplayer->OnClicked.AddDynamic(this, &ThisClass::MultiplayerBtnClicked);
	}

	if (BtnQuitGame)
	{
		BtnQuitGame->OnClicked.AddDynamic(this, &ThisClass::QuitGameBtnClicked);
	}

	if (BtnHost)
	{
		BtnHost->OnClicked.AddDynamic(this, &ThisClass::HostBtnClicked);
	}

	if (BtnSearch)
	{
		BtnSearch->OnClicked.AddDynamic(this, &ThisClass::SearchBtnClicked);
	}

	if (BtnConnection)
	{
		BtnConnection->OnClicked.AddDynamic(this, &ThisClass::ConnectionBtnClicked);
	}

	if (BtnBack)
	{
		BtnBack->OnClicked.AddDynamic(this, &ThisClass::BackBtnClicked);
	}

	if (BtnJoin)
	{
		BtnJoin->OnClicked.AddDynamic(this, &ThisClass::JoinBtnClicked);
	}

	if (BtnLaunch) 
	{
		BtnLaunch->OnClicked.AddDynamic(this, &ThisClass::LaunchBtnClicked);
		BtnLaunch->SetIsEnabled(false);
	}

	return true;
}

void UMainMenuWidget::HostBtnClicked()
{
	ChangeMapAndSearchVisibility(true, false);
}

void UMainMenuWidget::JoinBtnClicked()
{
	OnJoinButtonPressed.ExecuteIfBound("");
	//ToDo: Add the connectionSelected
}

void UMainMenuWidget::LaunchBtnClicked()
{
	if (CurrentMapSelectedName != "")
	{
		OnLaunchButtonPressed.ExecuteIfBound(CurrentMapSelectedName);
	}
}

void UMainMenuWidget::MapSelected(FString MapSelectedName)
{
	CurrentMapSelectedName = MapSelectedName;

	if (BtnLaunch)
	{
		BtnLaunch->SetIsEnabled(CurrentMapSelectedName != "");
	}

	UE_LOG(LogTemp, Warning, TEXT("MapSelected"));
}

void UMainMenuWidget::MenuDismiss()
{
	OnSoloGameButtonPressed.Unbind();
	OnExitGameButtonPressed.Unbind();
	OnSearchButtonPressed.Unbind();
	OnConnectionButtonPressed.Unbind();
	OnJoinButtonPressed.Unbind();
	OnLaunchButtonPressed.Unbind();

	OnMenuDismissButtonPressed.ExecuteIfBound();
	OnMenuDismissButtonPressed.Unbind();
}

void UMainMenuWidget::ChangeMapAndSearchVisibility(bool IsHostPressed, bool IsSearchPressed)
{
	ESlateVisibility HostSlateVisibility = IsHostPressed ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
	ESlateVisibility SearchSlateVisibility = IsSearchPressed ? ESlateVisibility::Visible : ESlateVisibility::Hidden;

	BtnHost->SetIsEnabled(!IsHostPressed);
	BtnSearch->SetIsEnabled(!IsSearchPressed);

	BtnLaunch->SetVisibility(HostSlateVisibility);

	MapSelectorPanel->SetVisibility(HostSlateVisibility);
	GameSearchList->SetVisibility(SearchSlateVisibility);
}

void UMainMenuWidget::NativeDestruct()
{
	MenuDismiss();
	Super::NativeDestruct();
}

void UMainMenuWidget::SoloGameBtnClicked()
{
	OnSoloGameButtonPressed.ExecuteIfBound("PortMap");
}

void UMainMenuWidget::MultiplayerBtnClicked()
{
	MainMenuPanel->SetVisibility(ESlateVisibility::Hidden);
	MultiplayerPanel->SetVisibility(ESlateVisibility::Visible);
}

void UMainMenuWidget::QuitGameBtnClicked()
{
	OnExitGameButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::SearchBtnClicked()
{
	ChangeMapAndSearchVisibility(false, true);

	OnSearchButtonPressed.ExecuteIfBound();
}

void UMainMenuWidget::ConnectionBtnClicked()
{
	OnConnectionButtonPressed.ExecuteIfBound();
	//ToDo: Add an IF for change Lan or Steam
}

void UMainMenuWidget::BackBtnClicked()
{
	MainMenuPanel->SetVisibility(ESlateVisibility::Visible);
	MultiplayerPanel->SetVisibility(ESlateVisibility::Hidden);
	ChangeMapAndSearchVisibility(false, false);
}

TSharedRef<SWidget, ESPMode::ThreadSafe> UMainMenuWidget::GetWidgetPrt()
{
	return TakeWidget();
}

void UMainMenuWidget::SetMapGame(TMap<FString, UTexture2D*> NewMaps, TSubclassOf<UMapSelectorCell> MapCellClass)
{
	if (NewMaps.Num() > 0)
	{
		TArray<FString> MapNames;
		NewMaps.GetKeys(MapNames);

		for (FString MapName : MapNames)
		{
			CreateMapSelectCell(MapName, NewMaps[MapName], MapCellClass);
		}
	}
}

void UMainMenuWidget::CreateMapSelectCell(FString MapName, UTexture2D* MapImage, TSubclassOf<UMapSelectorCell> MapCellClass)
{
	if (MapSelectorHBox)
	{
		UMapSelectorCell* MapCellWidget = CreateWidget<UMapSelectorCell>(this, MapCellClass);
		MapCellWidget->ConfigureCell(MapName, MapImage);
		MapCellWidget->OnMapButtonPressed.BindUObject(this, &ThisClass::MapSelected);
		MapSelectorHBox->AddChildToHorizontalBox(MapCellWidget);
	}
}

/*void UMainMenuWidget::ShowMessageInGame(FString Message, FColor Color)
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			Color,
			Message
		);
	}
}*/