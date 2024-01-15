// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "BoxNode.generated.h"

class UMaterialInstanceDynamic;

DECLARE_DELEGATE_TwoParams(FOnBoxHit, ABoxNode*, AActor*);

UCLASS()
class PYRAMIDPROJECT_API ABoxNode : public AActor
{
	GENERATED_BODY()

public:
	ABoxNode();

public:
	FOnBoxHit OnBoxHit;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Components)
	UParticleSystem* ExplosionParticles;

	UPROPERTY(EditAnywhere, Category = Components)
	UStaticMeshComponent* MeshComponent;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentColorType)
	FColor CurrentColorType;

	/* Box Material */
	UMaterialInstanceDynamic* BoxMaterial;

	/* Current Pyramid Position */
	int PyramidPosX;
	int PyramidPosZ;

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	FColor GetColor();

	void SetPyramidPos(int PosX, int PosZ);
	TTuple<int, int> GetPyramidPos();

	void TakeHitDamage(AActor* DamageActor);

	UFUNCTION(Server, Reliable)
	void SetColor(FColor ColorType);

	UFUNCTION(NetMulticast, Reliable)
	void DestroyWithParticle();

private:
	void ChangeMeshColor();

	UFUNCTION()
	void OnRep_CurrentColorType();
};