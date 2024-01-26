// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
class UBorder;

DECLARE_DELEGATE(FOnButtonPressed);
DECLARE_DELEGATE_OneParam(FOnJoinButtonPressed, FString, name);

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FOnButtonPressed OnHostButtonPressed;
	FOnButtonPressed OnMenuDismissButtonPressed;
	FOnButtonPressed OnSoloGameButtonPressed;
	FOnButtonPressed OnExitGameButtonPressed;
	FOnButtonPressed OnSearchButtonPressed;
	FOnButtonPressed OnConnectionButtonPressed;

	FOnJoinButtonPressed OnJoinButtonPressed;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSoloGame;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnMultiplayer;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnQuitGame;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnHost;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnSearch;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnConnection;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnBack;

	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnJoin;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* MainMenuPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* MultiplayerPanel;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* GameSearchList;

	UPROPERTY(meta = (BindWidgetOptional))
	UBorder* MapSelectorPanel;

protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;

private:
	UFUNCTION()
	void SoloGameBtnClicked();

	UFUNCTION()
	void MultiplayerBtnClicked();

	UFUNCTION()
	void QuitGameBtnClicked();

	UFUNCTION()
	void SearchBtnClicked();

	UFUNCTION()
	void ConnectionBtnClicked();

	UFUNCTION()
	void BackBtnClicked();

	UFUNCTION()
	void HostBtnClicked();

	UFUNCTION()
	void JoinBtnClicked();

	void MenuDismiss();
};
