// Fill out your copyright notice in the Description page of Project Settings.

#include "Zombie.h"
#include "Health.h"
#include "ZombieGameState.h"

class AGameState;
// Sets default values
AZombie::AZombie()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Collider = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	RootComponent = Collider;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
	Mesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	Health = CreateDefaultSubobject<UHealth>(TEXT("HealthComponent"));
	Health->OnDead.AddDynamic(this, &AZombie::DeactivateZombie);
}

// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

void AZombie::NotifyHitByRaycast(float firingDamage) const
{
	Health->AddHealth(-firingDamage);
}

void AZombie::DeactivateZombie()
{
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
	SetActorTickEnabled(false);
	AGameStateBase* GameState = GetWorld()->GetGameState();
	if (AZombieGameState* ZombieGame = Cast<AZombieGameState>(GameState))
	{
		ZombieGame->AddKill();
	}
}

