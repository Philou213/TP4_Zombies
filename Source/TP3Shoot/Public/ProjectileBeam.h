// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NiagaraComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "ProjectileBeam.generated.h"


UCLASS()
class TP3SHOOT_API AProjectileBeam : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, Category = "Components")
	UNiagaraComponent* NiagaraEffect;
	
	UPROPERTY(EditAnywhere, Category = "Timer")
	float BeamDuration = 0.1f;
	
public:	
	// Sets default values for this actor's properties
	AProjectileBeam();
	void Activates(float distance, int teamId);



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleSystemFinished(UNiagaraComponent* FinishedComponent);

private:
	void Deactivates();
	
};
