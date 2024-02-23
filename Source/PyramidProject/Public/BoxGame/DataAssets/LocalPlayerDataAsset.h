// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LocalPlayerDataAsset.generated.h"

USTRUCT()
struct PYRAMIDPROJECT_API FLocalPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString DefaultName;

	UPROPERTY()
	FString LocalName{ DefaultName };

	UPROPERTY()
	FString ID;
};

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API ULocalPlayerDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	struct FLocalPlayerInfo LocalPlayerInfo;
};
