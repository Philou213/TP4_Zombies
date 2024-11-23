// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectilePool.h"
#include "GameFramework/Character.h"
#include "AICharacter.generated.h"

UCLASS(config = Game)
class TP3SHOOT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AAICharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Input)
	float TurnRateGamepad;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Firing)
	float FiringRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Firing)
	float FiringForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Firing)
	AProjectilePool* BeamPool;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* TreeAsset;

protected:

	// Add a gun skeletal mesh component
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	class USkeletalMeshComponent* SK_Gun;

	// Particle Start
	UPROPERTY(EditAnywhere, Category = Gameplay)
	class UParticleSystem* ParticleStart;

	// Particle Impact
	UPROPERTY(EditAnywhere, Category = Gameplay)
	class UParticleSystem* ParticleImpact;

	// Fire animation
	UPROPERTY(EditAnywhere, Category = Gameplay)
	class UAnimMontage* FireAnimation;

	// Timer for Boost Speed
	FTimerHandle BoostSpeedTimer;

public:

	// Is Aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	bool IsAiming;

	// Is Firing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firing")
	bool IsFiring;

private:
	FTimerHandle FireTimer;
	void CheckIfFiringApplyForce(const AActor* otherActor, const FVector firingLocation, const FVector hitLocation);

};
