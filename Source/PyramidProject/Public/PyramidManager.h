// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PyramidManager.generated.h"

class ABoxNode;

DECLARE_DELEGATE(FOnPyramidDestroyed);
DECLARE_DELEGATE_TwoParams(FOnPyramidChange, int, AActor*);

UCLASS()
class PYRAMIDPROJECT_API APyramidManager : public AActor
{
	GENERATED_BODY()
	
public:
	FOnPyramidChange OnPyramidChange;
	FOnPyramidDestroyed OnPyramidDestroyed;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoxNode> BoxNodeClass;

	UPROPERTY(EditAnywhere)
	int BoxHeight{ 105 };

	UPROPERTY(EditAnywhere)
	int8 PyramidRow { 7 };

	UPROPERTY(EditAnywhere)
	TArray<FColor> ColorTypes;

	TArray<ABoxNode*> BoxNodes;

public:
	APyramidManager();

protected:
	virtual void BeginPlay() override;

private:
	UFUNCTION(Server, Reliable)
		void CreatePyramid();
	UFUNCTION(Server, Reliable)
		void ChangePyramidPos();

	void OnBoxDestroy(ABoxNode* BoxToDestroy, AActor* DamageActor);
	int DestroyBoxes(ABoxNode* FirstNode);
};
