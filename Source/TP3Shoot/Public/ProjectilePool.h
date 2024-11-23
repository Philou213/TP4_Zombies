#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ProjectilePool.generated.h"

UCLASS()
class TP3SHOOT_API AProjectilePool : public AActor
{
	GENERATED_BODY()

public:
	AProjectilePool();

	// Number of throwables to pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	int NbOfThrowable;

	// The class of throwable objects to pool
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Pooling")
	TSubclassOf<AActor> ThrowableClass;

	// Array to hold the pool of throwables
	TArray<AActor*> ThrowablesPoolArray;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Initialize the pool
	void InitializePool();

public:
	// Get a throwable from the pool
	AActor* GetThrowable();
};
