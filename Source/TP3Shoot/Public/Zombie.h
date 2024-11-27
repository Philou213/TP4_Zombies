// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Zombie.generated.h"

UCLASS()
class TP3SHOOT_API AZombie : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	class UCapsuleComponent* CapsuleCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UStaticMeshComponent* StaticMesh;

public:
	// Sets default values for this character's properties
	AZombie();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Health, meta = (AllowPrivateAccess = "true"))
	class UHealth* Health;

	void NotifyHitByRaycast(float firingDamage) const;

	UFUNCTION(BlueprintCallable)
	void KillZombie();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
