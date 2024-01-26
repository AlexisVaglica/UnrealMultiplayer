// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/MainMenuGameMode.h"
#include "BoxGame/HUD/MainMenu/MainMenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void AMainMenuGameMode::BeginPlay() 
{
	Super::BeginPlay();

	MainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	MainMenuWidget->AddToViewport();
	ConfigureMainMenuWidget();
}

void AMainMenuGameMode::ConfigureMainMenuWidget()
{
	if (!MainMenuWidget) 
	{
		return;
	}

	MainMenuWidget->OnExitGameButtonPressed.BindUObject(this, &AMainMenuGameMode::QuitGame);
}

void AMainMenuGameMode::QuitGame()
{
	UKismetSystemLibrary::QuitGame(GetWorld(), 0, EQuitPreference::Quit, false);
}