// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/MainMenu/MapSelectorCell.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Border.h"
#include "Internationalization/Text.h"

void UMapSelectorCell::ConfigureCell(FString MapName, UTexture2D* MapImage)
{
	CurrentMapName = MapName;

	FText NewMapName = FText::FromString(MapName);
	TextMapName->SetText(NewMapName);

	FString FullPath = MapImage->GetPathName();
	UTexture2D* LoadTexture = LoadObject<UTexture2D>(nullptr, *FullPath);

	if (LoadTexture)
	{
		ImageMap->SetBrushFromTexture(LoadTexture);
	}

	if (BtnMap)
	{
		BtnMap->OnClicked.AddDynamic(this, &ThisClass::SelectMapBtnClicked);
	}
}

void UMapSelectorCell::ConfigureCellOnline(bool IsHost)
{
	if (BtnMap) 
	{
		ESlateVisibility BtnVisibility = IsHost ? ESlateVisibility::Visible : ESlateVisibility::Hidden;
		BtnMap->SetVisibility(BtnVisibility);
	}
}

void UMapSelectorCell::SelectMap() 
{
	BGBorder->SetBrushColor(FLinearColor::Red);
}

void UMapSelectorCell::DeselectCell()
{
	BGBorder->SetBrushColor(FLinearColor::Transparent);
}

bool UMapSelectorCell::CompareMapName(FString MapName)
{
	bool AreEqual = CurrentMapName.Compare(MapName, ESearchCase::IgnoreCase) == 0;
	return AreEqual;
}

void UMapSelectorCell::SelectMapBtnClicked()
{
	BGBorder->SetBrushColor(FLinearColor::Red);
	OnMapButtonPressed.ExecuteIfBound(CurrentMapName);
}
