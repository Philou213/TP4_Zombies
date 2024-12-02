// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MassStateTreeTypes.h"
#include "MW_GetPlayerPosition.generated.h"

class UMassSignalSubsystem;

USTRUCT()
struct MASSWANDERAI_API FMW_FindPlayerLocationTaskInstanceData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category=Output)
	FVector OutLocation;

	FMW_FindPlayerLocationTaskInstanceData() = default;
};

/**
 * Find player location to wander to
 */
USTRUCT(meta = (DisplayName = "MW Find Player Location"))
struct MASSWANDERAI_API FMW_FindPlayerLocationTask : public FMassStateTreeTaskBase
{
	GENERATED_BODY()

	using FInstanceDataType = FMW_FindPlayerLocationTaskInstanceData;

	virtual bool Link(FStateTreeLinker& Linker) override;
	virtual const UStruct* GetInstanceDataType() const override { return FMW_FindPlayerLocationTaskInstanceData::StaticStruct(); }

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition) const override;

};
