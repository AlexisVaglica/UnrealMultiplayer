// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Utils/PlayerSettings.h"
#include "JsonObjectConverter.h"

TSharedPtr<FJsonObject> UPlayerSettings::ConvertDataToJsonObject()
{
	return FJsonObjectConverter::UStructToJsonObject(LocalPlayerInfo);
}

bool UPlayerSettings::ConvertJsonObjectToData(TSharedPtr<FJsonObject> JsonObject)
{
	return FJsonObjectConverter::JsonObjectToUStruct(JsonObject.ToSharedRef(), &LocalPlayerInfo);
}

