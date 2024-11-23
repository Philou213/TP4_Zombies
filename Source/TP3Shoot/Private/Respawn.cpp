// Fill out your copyright notice in the Description page of Project Settings.


#include "Respawn.h"

// Sets default values
ARespawn::ARespawn()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ARespawn::BeginPlay()
{
	Super::BeginPlay();
	
}

