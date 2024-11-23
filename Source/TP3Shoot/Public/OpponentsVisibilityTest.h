// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTest.h"
#include "OpponentsVisibilityTest.generated.h"

/**
 * 
 */
UCLASS()
class TP3SHOOT_API UOpponentsVisibilityTest : public UEnvQueryTest
{
	GENERATED_BODY()

protected:
	virtual void RunTest(FEnvQueryInstance& QueryInstance) const override;
	
};
