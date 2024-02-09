// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MapDataAsset.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UMapDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	TSoftObjectPtr<UWorld> Map;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	FText GameMapName;

	UPROPERTY(EditAnywhere, Category = "Game Data")
	TSoftObjectPtr<UTexture2D> MapImage;
};
