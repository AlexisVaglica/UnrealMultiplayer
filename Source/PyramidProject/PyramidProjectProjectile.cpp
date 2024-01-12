// Copyright Epic Games, Inc. All Rights Reserved.

#include "PyramidProjectProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Public\BoxNode.h"
#include "Components/SphereComponent.h"
#include "PyramidProjectCharacter.h"

APyramidProjectProjectile::APyramidProjectProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &APyramidProjectProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	// I Call the function when this projectile was Destroyed
	this->OnDestroyed.AddDynamic(this, &APyramidProjectProjectile::OnDestroyedActor);
}

void APyramidProjectProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		/* if the otherActor is a ABoxTarget class, call the method in ABoxTarget for Damage */
		if(OtherActor->IsA(ABoxNode::StaticClass()))
		{
			APlayerController* PlayerController = OwnerReference->GetPlayerController();

			ABoxNode* BoxTarget = Cast<ABoxNode>(OtherActor);
			BoxTarget->TakeHitDamage(PlayerController);
		}

		Destroy();
	}
}

void APyramidProjectProjectile::SetPlayerOwner(APyramidProjectCharacter* PlayerOwner)
{
	OwnerReference = PlayerOwner;
}

void APyramidProjectProjectile::OnDestroyedActor(AActor* Actor)
{
	OnBulletWasDestroyed.ExecuteIfBound();
}