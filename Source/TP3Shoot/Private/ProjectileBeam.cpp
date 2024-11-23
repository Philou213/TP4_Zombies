// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectileBeam.h"

// Sets default values
AProjectileBeam::AProjectileBeam()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set Niagara Effect as the root component
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraEffect"));
	RootComponent = NiagaraEffect;

	NiagaraEffect->OnSystemFinished.AddDynamic(this, &AProjectileBeam::HandleSystemFinished);
}

void AProjectileBeam::Activates(float distance, int teamId)
{
	NiagaraEffect->SetVectorParameter("BeamEnd", FVector(distance,0,0));
	if (teamId == 0)
		NiagaraEffect->SetColorParameter("BeamColor", FColor::Blue);
	else
		NiagaraEffect->SetColorParameter("BeamColor", FColor::Red);
	NiagaraEffect->Activate();
	SetActorHiddenInGame(false);
}

// Called when the game starts or when spawned
void AProjectileBeam::BeginPlay()
{
	Super::BeginPlay();
	NiagaraEffect->SetFloatParameter("BeamDuration", BeamDuration);
}

void AProjectileBeam::HandleSystemFinished(UNiagaraComponent* FinishedComponent)
{
	Deactivates();
}


void AProjectileBeam::Deactivates()
{
	SetActorHiddenInGame(true);
}



