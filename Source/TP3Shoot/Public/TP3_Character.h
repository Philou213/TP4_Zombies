// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TP3_Character.generated.h"

UCLASS()
class TP3SHOOT_API ATP3_Character : public ACharacter
{
	GENERATED_BODY()


public:
	// Sets default values for this character's properties
	ATP3_Character();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
