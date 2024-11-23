// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Respawn.generated.h"

UCLASS()
class TP3SHOOT_API ARespawn : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARespawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	

};
