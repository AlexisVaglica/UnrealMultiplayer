// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ScoreCellWidget.generated.h"

class UTextBlock;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UScoreCellWidget : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta=(BindWidget))
	UTextBlock* Txt_PlayerScore;
public:
	void SetupCell(FString PlayerName, float Score);

};
