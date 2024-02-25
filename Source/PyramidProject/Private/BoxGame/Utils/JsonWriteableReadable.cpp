// Fill out your copyright notice in the Description page of Project Settings.


#include "BoxGame/Utils/JsonWriteableReadable.h"

TSharedPtr<FJsonObject> IJsonWriteableReadable::ConvertDataToJsonObject()
{
    return TSharedPtr<FJsonObject>();
}

bool IJsonWriteableReadable::ConvertJsonObjectToData(TSharedPtr<FJsonObject> JsonObject) 
{
    return false;
}