// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Utils/ReadWriteJsonFile.h"
#include "BoxGame/Utils/ReadWriteFile.h"
#include "Serialization/JsonSerializer.h"
#include "JsonObjectConverter.h"

UScriptStruct* UReadWriteJsonFile::ReadJson(FString JsonPath, bool& bSuccess, FString& OutInfoMessage)
{
	FString JsonString = UReadWriteFile::ReadStringFromFile(JsonPath, bSuccess, OutInfoMessage);

	if (!bSuccess)
	{
		return nullptr;
	}

	TSharedPtr<FJsonObject> JsonObject;

	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), JsonObject))
	{
		bSuccess = false;
		OutInfoMessage = FString(TEXT("Read Json Failed - Was not able to deserialize json string"));
		return nullptr;
	}

	UScriptStruct* ResultObject = ThisClass::JsonToObject(JsonObject);

	if (ResultObject == nullptr)
	{
		bSuccess = false;
		OutInfoMessage = FString(TEXT("Read Json Failed - Was not able to create Object from json"));
		return nullptr;
	}

	bSuccess = true;
	OutInfoMessage = FString(TEXT("Read Json Succeded!"));

	return ResultObject;
}

void UReadWriteJsonFile::WriteJson(FString JsonPath, const UScriptStruct* ObjectToSave, bool& bSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> JsonObject = ThisClass::ObjectToJson(ObjectToSave);

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
	OutInfoMessage = FString(TEXT("Write Json Succeded!"));
}

UScriptStruct* UReadWriteJsonFile::JsonToObject(TSharedPtr<FJsonObject> JsonObject)
{
	UScriptStruct* ResultObject;

	if (!FJsonObjectConverter::JsonObjectToUStruct<UScriptStruct*>(JsonObject.ToSharedRef(), &ResultObject))
	{
		return nullptr;
	}

	return nullptr;
}

TSharedPtr<FJsonObject> UReadWriteJsonFile::ObjectToJson(const UScriptStruct* Object)
{
	auto JsonObject = FJsonObjectConverter::UStructToJsonObject(Object);
	return JsonObject.ToSharedRef();
}
