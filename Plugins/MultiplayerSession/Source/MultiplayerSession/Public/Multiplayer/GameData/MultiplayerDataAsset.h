// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MultiplayerDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERSESSION_API UMultiplayerDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = "Game Data")
	FText GameTypeName;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	FText GameDescription;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	int32 MaxPlayersCount;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	TSoftObjectPtr<UTexture2D> MapImage;

	FString SessionName = FString(TEXT("Box Game"));
	bool IsLanMatch;
};
