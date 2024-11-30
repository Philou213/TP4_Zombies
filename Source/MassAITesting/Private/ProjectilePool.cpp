// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectilePool.h"

AProjectilePool::AProjectilePool()
{
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void AProjectilePool::BeginPlay()
{
	Super::BeginPlay();

	// Initialize the pool
	InitializePool();
}

// Initialize the pool by spawning throwables
void AProjectilePool::InitializePool()
{
	if (!ThrowableClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("ThrowableClass is not set in the pool"));
		return;
	}

	for (int32 i = 0; i < NbOfThrowable; ++i)
	{
		AActor* NewThrowable = GetWorld()->SpawnActor<AActor>(ThrowableClass);
		if (NewThrowable)
		{
			NewThrowable->SetActorHiddenInGame(true);  // Hide initially
			NewThrowable->SetActorEnableCollision(false);  // Disable collision
			NewThrowable->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			ThrowablesPoolArray.Add(NewThrowable);
		}
	}
}

// Get a throwable from the pool
AActor* AProjectilePool::GetThrowable()
{
	for (AActor* Throwable : ThrowablesPoolArray)
	{
		if (Throwable && Throwable->IsHidden())
		{
			return Throwable;
		}
	}
	UE_LOG(LogTemp, Warning, TEXT("No throwable available in the pool"));
	return nullptr;
}


