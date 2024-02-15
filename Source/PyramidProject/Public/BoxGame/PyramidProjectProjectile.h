// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PyramidProjectProjectile.generated.h"

DECLARE_DELEGATE(FBulletWasDestroyedSignature);

class APyramidProjectCharacter;

UCLASS(config=Game)
class APyramidProjectProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditAnywhere)
	float MaxLifeTime{ 3.f };

private:
	UPROPERTY()
	APyramidProjectCharacter* OwnerReference;

	UFUNCTION()
	void OnDestroyedActor(AActor* Actor);

public:
	APyramidProjectProjectile();

	FBulletWasDestroyedSignature OnBulletWasDestroyed;

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	FORCEINLINE float GetBulletLifeTime() const { return MaxLifeTime; };

	void SetPlayerOwner(APyramidProjectCharacter* PlayerOwner);

};

