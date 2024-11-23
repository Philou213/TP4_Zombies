// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Respawn.h"
#include "Dead.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TP3SHOOT_API UDead : public UActorComponent
{
	GENERATED_BODY()


	

public:	
	// Sets default values for this component's properties
	UDead();
	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, category="Respawn")
	float RespawnCooldown;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UFUNCTION()
	void DeactivateCharacter();
	
	void ReactivateCharacter();
	FTimerHandle RespawnTimer;
	ARespawn* Respawn;
};
