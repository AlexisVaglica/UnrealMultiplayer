// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteJsonFile.generated.h"

class FJsonObject;

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UReadWriteJsonFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static UScriptStruct* ReadJson(FString JsonPath, bool& bSuccess, FString& OutInfoMessage);
	static void WriteJson(FString JsonPath, const UScriptStruct* ObjectToSave, bool& bSuccess, FString& OutInfoMessage);

private:
	static UScriptStruct* JsonToObject(TSharedPtr<FJsonObject> JsonObject);
	static TSharedPtr<FJsonObject> ObjectToJson(const UScriptStruct* Object);
};