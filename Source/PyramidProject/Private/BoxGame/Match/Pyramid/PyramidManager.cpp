// Fill out your copyright notice in the Description page of Project Settings.

#include "BoxGame/Match/Pyramid/PyramidManager.h"
#include "BoxGame/Match/Pyramid/BoxNode.h"

APyramidManager::APyramidManager()
{
	PrimaryActorTick.bCanEverTick = false;
}

void APyramidManager::BeginPlay()
{
	Super::BeginPlay();

	if (ColorTypes.Num() <= 0) 
	{
		FColor ColorInstance;
		ColorTypes.Add(ColorInstance.Red);
		ColorTypes.Add(ColorInstance.Green);
		ColorTypes.Add(ColorInstance.Blue);
	}
	
	CreatePyramid();
}

int APyramidManager::DestroyBoxes(ABoxNode* FirstNode)
{
	/* Add the box to destroy in an array */
	TArray<ABoxNode*> BoxesToDestroy;
	BoxesToDestroy.Add(FirstNode);

	int BoxCount = 0;

	/* Destroy all the nodes at the same color */
	while (BoxesToDestroy.Num() > 0) {
		ABoxNode* BoxNode = BoxesToDestroy[0];

		// Current Position of the Box to Destroy
		auto BoxNodeCurrentPos = BoxNode->GetPyramidPos();

		// count of Boxes who was destroyed
		BoxCount++;

		/* Search for neighbors boxes of the same color */
		for (ABoxNode* &NewBoxNode : BoxNodes)
		{
			/* Search for adjacent Boxes */
			auto newBoxNodeCurrentPos = NewBoxNode->GetPyramidPos();
			if (newBoxNodeCurrentPos.Key == BoxNodeCurrentPos.Key - 1 && newBoxNodeCurrentPos.Value == BoxNodeCurrentPos.Value
				|| newBoxNodeCurrentPos.Key == BoxNodeCurrentPos.Key + 1 && newBoxNodeCurrentPos.Value == BoxNodeCurrentPos.Value
				|| newBoxNodeCurrentPos.Key == BoxNodeCurrentPos.Key     && newBoxNodeCurrentPos.Value == BoxNodeCurrentPos.Value - 1
				|| newBoxNodeCurrentPos.Key == BoxNodeCurrentPos.Key     && newBoxNodeCurrentPos.Value == BoxNodeCurrentPos.Value + 1)
			{
				/* If is adjacent Box and is it the same color, add to BoxesToDestroy */
				if (NewBoxNode->GetColor() == BoxNode->GetColor() && BoxesToDestroy.Contains(NewBoxNode) == false)
				{
					BoxesToDestroy.Add(NewBoxNode);
				}
			}
		}

		/* Destroy the current node */
		if (BoxNode != nullptr)
		{
			BoxNodes.Remove(BoxNode);
			BoxesToDestroy.Remove(BoxNode);
			BoxNode->DestroyWithParticle();
		}
	}

	ChangePyramidPos();

	return BoxCount;
}

void APyramidManager::CreatePyramid_Implementation()
{
	/* ColorArrSize is the count of colors in FColorTypes */
	const int ColorArrSize{ ColorTypes.Num() };

	int i{}, j{};
	const FRotator BoxRotation{ FRotator(0, 0, 0) };

	for (i = 0; i <= (PyramidRow - 1); i++)
	{
		for (j = 0; j <= (PyramidRow - 1); j++)
		{
			if (j >= (i / 2) && j <= ((PyramidRow - 1) - (i / 2)))
			{
				const int PosX{ -(BoxHeight * j) };
				const int PosZ{ BoxHeight * i };
				FVector BoxPosition{ FVector(PosX, 0, PosZ) };
				ABoxNode* MyBoxNode{ GetWorld()->SpawnActor<ABoxNode>(BoxNodeClass, BoxPosition, BoxRotation) };

				if (MyBoxNode != nullptr) {
					const int RandomNumber{ rand() % ColorArrSize };
					const FColor CurrentColor{ APyramidManager::ColorTypes[RandomNumber] };

					MyBoxNode->SetColor(CurrentColor);
					MyBoxNode->SetPyramidPos(j, i);
					MyBoxNode->OnBoxHit.BindUObject(this, &APyramidManager::OnBoxDestroy);
					BoxNodes.Add(MyBoxNode);
				}
			}
		}
	}
}

void APyramidManager::ChangePyramidPos_Implementation()
{
	TArray<ABoxNode*> VisitedBoxed = BoxNodes;
	while (VisitedBoxed.Num() > 0)
	{
		auto CurrentNode = VisitedBoxed[0];
		auto CurrentPos = CurrentNode->GetPyramidPos();

		// This Search a BoxNode under another if there not any, the current node change to a new position in the Pyramid
		if (BoxNodes.ContainsByPredicate([&, CurrentPos](ABoxNode* BoxNode)
		{
			auto BoxNodePos = BoxNode->GetPyramidPos();
			return BoxNodePos.Value == CurrentPos.Value - 1 && BoxNodePos.Key == CurrentPos.Key || CurrentPos.Value - 1 < 0;
		}))
		{
			VisitedBoxed.Remove(CurrentNode);
			continue;
		}
		else
		{
			CurrentNode->SetPyramidPos(CurrentPos.Key, CurrentPos.Value - 1);
		}
	}
}

void APyramidManager::OnBoxDestroy(ABoxNode* BoxToDestroy, AActor* DamageActor)
{
	const int BoxCount{ DestroyBoxes(BoxToDestroy) };
	OnPyramidChange.ExecuteIfBound(BoxCount, DamageActor);

	/* Verify is GameOver */
	if (BoxNodes.Num() <= 0) {
		OnPyramidDestroyed.ExecuteIfBound();

		GameOverDismiss();
	}
}

void APyramidManager::GameOverDismiss()
{
	OnPyramidChange.Unbind();
	OnPyramidDestroyed.Unbind();
}