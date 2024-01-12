// Fill out your copyright notice in the Description page of Project Settings.

#include "BoxNode.h"
#include "Math/Color.h"
#include "Net/UnrealNetwork.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/GameplayStatics.h"

ABoxNode::ABoxNode()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	RootComponent = MeshComponent;
}

void ABoxNode::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//TODO: Refactor
	MeshComponent->SetPhysicsLinearVelocity(FVector(0, 0, -10), true);
}

FColor ABoxNode::GetColor()
{
	return CurrentColorType;
}

void ABoxNode::SetPyramidPos(int PosX, int PosZ)
{
	PyramidPosX = PosX;
	PyramidPosZ = PosZ;
}

TTuple<int, int> ABoxNode::GetPyramidPos()
{
	return TTuple<int, int>(PyramidPosX, PyramidPosZ);
}

void ABoxNode::TakeHitDamage(AController* DamageActor)
{
	OnBoxHit.ExecuteIfBound(this, DamageActor);
}

void ABoxNode::ChangeMeshColor()
{
	BoxMaterial = MeshComponent->CreateAndSetMaterialInstanceDynamic(0);
	BoxMaterial->SetVectorParameterValue(FName(TEXT("DiffuseColor")), FLinearColor(CurrentColorType));
}

void ABoxNode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ABoxNode, CurrentColorType);
}

void ABoxNode::OnRep_CurrentColorType()
{
	ChangeMeshColor();
}

void ABoxNode::SetColor_Implementation(FColor ColorType)
{
	CurrentColorType = ColorType;

	if (GetLocalRole() == ROLE_Authority)
	{
		ChangeMeshColor();
	}
}

void ABoxNode::DestroyWithParticle_Implementation()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplosionParticles, GetActorLocation());
	Destroy();
}