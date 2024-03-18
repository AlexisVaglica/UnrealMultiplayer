// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "BoxGame/Utils/JsonWriteableReadable.h"
#include "PlayerSettings.generated.h"

USTRUCT()
struct PYRAMIDPROJECT_API FLocalPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString DefaultName{ "Box Player" };

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
	struct FLocalPlayerInfo LocalPlayerInfo;
	const FString PlayerSettingsPath{ FPaths::AutomationDir() + "PlayerSettings.json" };

public:
	virtual TSharedPtr<FJsonObject> ConvertDataToJsonObject() override;
	virtual bool ConvertJsonObjectToData(TSharedPtr<FJsonObject> JsonObject) override;
};
