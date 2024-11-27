// Fill out your copyright notice in the Description page of Project Settings.


#include "Zombie.h"

#include "Health.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AZombie::AZombie()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create BoxComponent and set as RootComponent for the Actor
	CapsuleCollision = CreateDefaultSubobject<UCapsuleComponent>("BoxCollision");
	RootComponent = CapsuleCollision;

	// Create StaticMeshComponent and Attach to BoxComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(CapsuleCollision);

	Health = CreateDefaultSubobject<UHealth>(TEXT("Health"));
	Health->OnDead.AddDynamic(this, &AZombie::KillZombie);

}

void AZombie::NotifyHitByRaycast(float firingDamage) const
{
	Health->AddHealth(-firingDamage);
}

void AZombie::KillZombie()
{
	//TODO: Disable instead of destroy
	Destroy();
}


// Called when the game starts or when spawned
void AZombie::BeginPlay()
{
	Super::BeginPlay();
	
}

