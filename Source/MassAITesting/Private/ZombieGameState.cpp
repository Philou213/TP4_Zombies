// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGameState.h"

AZombieGameState::AZombieGameState()
{
	GameElapsedTime = 0;
	TotalZombieKills = 0;
	PrimaryActorTick.bCanEverTick = false;
}

void AZombieGameState::AddKill()
{
	TotalZombieKills++;
	UpdateScore();
}

void AZombieGameState::UpdateScore()
{
	int Score = GameElapsedTime * 10 + TotalZombieKills * 10;
	OnScoreUpdated.Broadcast(Score);
}

void AZombieGameState::BeginPlay()
{
	Super::BeginPlay();
	StartTimer();
}

void AZombieGameState::StartTimer()
{
	GetWorldTimerManager().SetTimer(Timer, this, &AZombieGameState::UpdateTimer, 1, true);
}

void AZombieGameState::UpdateTimer()
{
	GameElapsedTime++;
	UpdateScore();
}
