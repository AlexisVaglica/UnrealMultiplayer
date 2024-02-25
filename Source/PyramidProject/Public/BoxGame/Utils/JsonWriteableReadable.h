// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "JsonWriteableReadable.generated.h"

class FJsonObject;

UINTERFACE(MinimalAPI)
class UJsonWriteableReadable : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PYRAMIDPROJECT_API IJsonWriteableReadable
{
	GENERATED_BODY()

public:
	virtual TSharedPtr<FJsonObject> ConvertDataToJsonObject();
	virtual bool ConvertJsonObjectToData(TSharedPtr<FJsonObject> JsonObject);
};
