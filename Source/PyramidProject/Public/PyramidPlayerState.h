// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PyramidPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class PYRAMIDPROJECT_API APyramidPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	void SetFibonacciScore(float ScoreDelta);
};
