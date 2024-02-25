// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteJsonFile.generated.h"

class FJsonObject;
class IJsonWriteableReadable;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UReadWriteJsonFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static void ReadJson(FString JsonPath, bool& bSuccess, FString& OutInfoMessage, IJsonWriteableReadable* OutObject);
	static void WriteJson(FString JsonPath, IJsonWriteableReadable* ObjectToSave, bool& bSuccess, FString& OutInfoMessage);
};

