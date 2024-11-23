// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"

#include "Components/BoxComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create BoxComponent and set as RootComponent for the Actor
	BoxCollision = CreateDefaultSubobject<UBoxComponent>("BoxCollision");
	RootComponent = BoxCollision;

	// Create StaticMeshComponent and Attach to BoxComponent
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxCollision);

	ProjectileComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComponent"));
	ProjectileComponent->UpdatedComponent = RootComponent;
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	
}

