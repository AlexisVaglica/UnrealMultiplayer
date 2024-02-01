// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MapSelectorCell.generated.h"

class UButton;
class UTextBlock;
class UImage;

DECLARE_DELEGATE_OneParam(FOnMapSelected, FString);

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UMapSelectorCell : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnMapSelected OnMapButtonPressed;

private:
	UPROPERTY(meta = (BindWidgetOptional))
	UButton* BtnMap;
	
	UPROPERTY(meta = (BindWidgetOptional))
	UTextBlock* TextMapName;

	UPROPERTY(meta = (BindWidgetOptional))
	UImage* ImageMap;

public:
	void ConfigureCell(FString MapName, UTexture2D* MapImage);

private:
	void SelectMapButtonPressed();
};
