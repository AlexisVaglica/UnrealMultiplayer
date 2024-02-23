// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ReadWriteFile.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API UReadWriteFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	static FString ReadStringFromFile(FString FilePath, bool& bSuccess, FString& OutInfoMessage);

	UFUNCTION()
	static void WriteStringToFile(FString FilePath, FString StringFile, bool& bSuccess, FString& OutInfoMessage);
};
