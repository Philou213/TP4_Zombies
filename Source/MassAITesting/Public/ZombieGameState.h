// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "ZombieGameState.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FScoreUpdated, int, Score);

UCLASS()
class MASSAITESTING_API AZombieGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	AZombieGameState();
	
	/** The total elapsed time since the game started */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Stats")
	float GameElapsedTime;

	/** The total number of zombie kills */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Stats")
	int TotalZombieKills;

	void AddKill();

	void UpdateScore() const;

	void StopUpdateScore();
	
	UPROPERTY(BlueprintAssignable, Category = "Game Stats")
	FScoreUpdated OnScoreUpdated;

private:
	virtual void BeginPlay() override;
	void StartTimer();
	void UpdateTimer();
	FTimerHandle Timer;
	bool InProgress;
};
