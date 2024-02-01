// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/HUD/MainMenu/MapSelectorCell.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Internationalization/Text.h"

void UMapSelectorCell::ConfigureCell(FString MapName, UTexture2D* MapImage)
{
	FText NewMapName = FText::FromString(MapName);
	TextMapName->SetText(NewMapName);

	ImageMap->SetBrushFromTexture(MapImage);

	BtnMap->OnClicked.AddDynamic(this, &ThisClass::SelectMapButtonPressed);
}

void UMapSelectorCell::SelectMapButtonPressed()
{
	FString SelectedMapName = TextMapName->GetText().ToString();
	OnMapButtonPressed.ExecuteIfBound(SelectedMapName);
}
