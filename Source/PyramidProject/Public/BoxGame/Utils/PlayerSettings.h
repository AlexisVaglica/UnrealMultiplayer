// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoxGame/Utils/JsonWriteableReadable.h"
#include "PlayerSettings.generated.h"

USTRUCT()
struct PYRAMIDPROJECT_API FLocalPlayerInfoo
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
class PYRAMIDPROJECT_API UPlayerSettings : public UObject, public IJsonWriteableReadable
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	struct FLocalPlayerInfoo LocalPlayerInfo;
	const FString PlayerSettingsPath{ "C:/Users/zorro/Documents/PlayerSettings.json" };

public:
	virtual TSharedPtr<FJsonObject> ConvertDataToJsonObject() override;
	virtual bool ConvertJsonObjectToData(TSharedPtr<FJsonObject> JsonObject) override;
};
