// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ProjectilePool.h"
#include "GameFramework/Character.h"
#include "ZombiePlayer.generated.h"

UCLASS()
class MASSAITESTING_API AZombiePlayer : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

public:
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void Deactivate();
	
	AZombiePlayer();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Trigger, meta = (AllowPrivateAccess = "true"))
	class USphereComponent* Trigger;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UHealth* Health;

	UPROPERTY(EditAnywhere, Category = "AI")
	class UBehaviorTree* TreeAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UAIPerceptionStimuliSourceComponent* StimuliSource;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Firing)
	float FiringRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Firing)
	float FiringForce;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Firing)
	float FiringDamage;

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

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// Aiming function
	void Aim();

	void StopAiming();

	FVector GetCameraRaycastHitLocation();

	void StartFiring();

	void StopFiring();
	
	void BoostSpeed();

	void RemoveSpeedBoost();

	void FireParticle(FVector Start, FVector Impact);

public:
	// Firing function
	UFUNCTION(BlueprintCallable)
	void Fire();

	void FireStraight();

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

public:

	// Is Aiming
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Aiming")
	bool IsAiming;

	// Is Firing
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Firing")
	bool IsFiring;

	void BeginPlay() override;

private:
	FTimerHandle FireTimer;
	void CheckIfZombie(const AActor* HitActor);
	void NotifyHitByRaycast(float firingDamage) const;
	AProjectilePool* BeamPool;
};
