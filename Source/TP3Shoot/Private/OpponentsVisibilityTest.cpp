// Fill out your copyright notice in the Description page of Project Settings.


#include "OpponentsVisibilityTest.h"

#include "Health.h"
#include "Kismet/GameplayStatics.h"
#include "TP3Shoot/TP3ShootCharacter.h"

void UOpponentsVisibilityTest::RunTest(FEnvQueryInstance& QueryInstance) const
{
	// Ensure the world exists
	UWorld* World = QueryInstance.World;
	if (!World)
	{
		return;
	}

	// Get all characters in the level
	TArray<AActor*> FoundCharacters;
	UGameplayStatics::GetAllActorsOfClass(World, ATP3ShootCharacter::StaticClass(), FoundCharacters);

	// Loop through each item in the query (e.g., location points)
	for (FEnvQueryInstance::ItemIterator It(this, QueryInstance); It; ++It)
	{
		FVector ItemLocation = GetItemLocation(QueryInstance, It);

		// Here you can perform custom checks using `FoundCharacters`
		float Score = 0.0f;
		for (AActor* Character : FoundCharacters)
		{
			if (Character)
			{
				// Perform a line trace from the character's location to the item location
				FHitResult HitResult;
				FCollisionQueryParams TraceParams(FName(TEXT("VisibilityTrace")), true, Character);
				TraceParams.bReturnPhysicalMaterial = false;
				TraceParams.AddIgnoredActor(Character); // Ignore the character itself in the trace

				bool bHit = World->LineTraceSingleByChannel(
				HitResult,
				Character->GetActorLocation(),
				ItemLocation,
				ECC_Visibility, // Use visibility channel for line of sight
				TraceParams
				);

				DrawDebugLine(World, Character->GetActorLocation(), ItemLocation, bHit ? FColor::Red : FColor::Green, false, 1.0f);

				// If no obstacle was hit, then the item location is visible
				if (!bHit)
				{
					// Increase score for visible locations
					float Distance = FVector::Dist(ItemLocation, Character->GetActorLocation());
					Score += 1.0f / (Distance + 1.0f); // Example scoring logic based on distance
				}
			}
		}

		// Set the score for the current item
		It.SetScore(TestPurpose, FilterType, Score, 0.0f, 1.0f);
	}
}
