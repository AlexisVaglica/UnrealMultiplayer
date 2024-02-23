// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Utils/ReadWriteFile.h"
#include "HAL/PlatformFileManager.h"
#include "Misc/FileHelper.h"

FString UReadWriteFile::ReadStringFromFile(FString FilePath, bool& bSuccess, FString& OutInfoMessage)
{
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath)) 
	{
		bSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read from file Failed - File doesn't exist"));
		return "";
	}

	FString ResultString = "";

	if (!FFileHelper::LoadFileToString(ResultString, *FilePath)) 
	{
		bSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Read from file Failed - Was not able to read file"));
		return "";
	}

	bSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read from file Succeded!"));

	return ResultString;
}

void UReadWriteFile::WriteStringToFile(FString FilePath, FString StringFile, bool& bSuccess, FString& OutInfoMessage)
{
	if (!FFileHelper::SaveStringToFile(StringFile, *FilePath)) 
	{
		bSuccess = false;
		OutInfoMessage = FString::Printf(TEXT("Write to file Failed - Was not able to write file"));
	}

	bSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write to file Succeded!"));
}
