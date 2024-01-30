// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"

class UMainMenuWidget;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	UMainMenuWidget* MainMenuWidget;

protected:
	virtual void BeginPlay() override;

private:
	void ConfigureMainMenuWidget();
	void LaunchGame(FString MapName);
	void QuitGame();

};
