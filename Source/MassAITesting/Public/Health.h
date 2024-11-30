// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Health.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthValueChanged, float, newHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDead);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MASSAITESTING_API UHealth : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UHealth();

	UPROPERTY(EditAnywhere, Category="Health")
	float MaxHealth;

	UPROPERTY(EditAnywhere, Category="StartHealthRegeneration")
	float HealthRegenerationStartCooldown;

	UPROPERTY(EditAnywhere, Category="HealthRegeneration")
	float HealthRegenerationRate;

	UPROPERTY(EditAnywhere, Category="HealthRegeneration")
	float HealthRegenerationRateCooldown;

	UPROPERTY(EditAnywhere, Category = "HealthRegeneration")
	bool IsHealthRegenerating = false;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnHealthValueChanged OnHealthValueChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnDead OnDead;



	void AddHealth(float Health);
	void StartHealthRegenerationCooldown();
	void HealthRegeneration();

	UFUNCTION(BlueprintCallable)
	float GetHealthPourcent();

	float CurrentHealth;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;


private:
	FTimerHandle HealthRegenerationStartTimer;
	FTimerHandle HealthRegenerationRateTimer;
	

		
};

