// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Utils/ReadWriteJsonFile.h"
#include "BoxGame/Utils/ReadWriteFile.h"
#include "BoxGame/Utils/JsonWriteableReadable.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

void UReadWriteJsonFile::ReadJson(FString JsonPath, bool& bSuccess, FString& OutInfoMessage, IJsonWriteableReadable* OutObject)
{
	FString JsonString = UReadWriteFile::ReadStringFromFile(JsonPath, bSuccess, OutInfoMessage);

	if (!bSuccess)
	{
		OutInfoMessage = FString(TEXT("Read Json Failed - Object was not found"));
		return;
	}

	TSharedPtr<FJsonObject> JsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject))
	{
		bSuccess = false;
		OutInfoMessage = FString(TEXT("Read Json Failed - Was not able to deserialize json string"));
		return;
	}

	bSuccess = OutObject->ConvertJsonObjectToData(JsonObject);

	if (!bSuccess)
	{
		OutInfoMessage = FString(TEXT("Read Json Failed - Was not able to create Object from json"));
		return;
	}

	bSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Read Json Succeded! Path: %s"), *JsonPath);
}

void UReadWriteJsonFile::WriteJson(FString JsonPath, IJsonWriteableReadable* ObjectToSave, bool& bSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> JsonObject = ObjectToSave->ConvertDataToJsonObject();

	if (JsonObject == nullptr)
	{
		bSuccess = false;
		OutInfoMessage = FString(TEXT("Write Json Failed - Was not able to create Json from Object"));
		return;
	}

	FString JsonString;

	if (!FJsonSerializer::Serialize( JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		bSuccess = false;
		OutInfoMessage = FString(TEXT("Write Json Failed - Was not able to serialize json string"));
		return;
	}

	UReadWriteFile::WriteStringToFile(JsonPath, JsonString, bSuccess, OutInfoMessage);

	if (!bSuccess)
	{
		return;
	}

	bSuccess = true;
	OutInfoMessage = FString::Printf(TEXT("Write Json Succeded! Path: %s"), *JsonPath);
}