// Fill out your copyright notice in the Description page of Project Settings.


#include "Health.h"

// Sets default values for this component's properties
UHealth::UHealth()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


void UHealth::AddHealth(float AddHealth)
{
	CurrentHealth += AddHealth;
	UE_LOG(LogTemp, Warning, TEXT("Current Health: %f"), CurrentHealth);
	
	if (CurrentHealth > MaxHealth)
	{
		CurrentHealth = MaxHealth;
		//Stop regeneration
		if (IsHealthRegenerating) GetWorld()->GetTimerManager().ClearTimer(HealthRegenerationRateTimer);
	}
	if (CurrentHealth <= 0)
	{
		CurrentHealth = MaxHealth;
		OnDead.Broadcast();
	}
	OnHealthValueChanged.Broadcast(CurrentHealth);
	if (IsHealthRegenerating && AddHealth < 0) StartHealthRegenerationCooldown();
}

//Wait a certain time before starting regenerate
void UHealth::StartHealthRegenerationCooldown()
{
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenerationStartTimer);
	GetWorld()->GetTimerManager().ClearTimer(HealthRegenerationRateTimer);
	GetWorld()->GetTimerManager().SetTimer(HealthRegenerationStartTimer, this, &UHealth::HealthRegeneration, HealthRegenerationStartCooldown);
}

//Regenerate the health progressively
void UHealth::HealthRegeneration()
{
	GetWorld()->GetTimerManager().SetTimer(HealthRegenerationRateTimer, this, &UHealth::HealthRegeneration, HealthRegenerationRateCooldown, true);
	AddHealth(HealthRegenerationRate);
}

float UHealth::GetHealthPourcent()
{
	return CurrentHealth * 100 / MaxHealth;
}

// Called when the game starts
void UHealth::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
	
}

