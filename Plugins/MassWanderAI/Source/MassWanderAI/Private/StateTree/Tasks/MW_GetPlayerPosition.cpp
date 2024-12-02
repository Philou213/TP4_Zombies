// Fill out your copyright notice in the Description page of Project Settings.


#include "StateTree/Tasks/MW_GetPlayerPosition.h"
#include "StateTreeExecutionContext.h"

bool FMW_FindPlayerLocationTask::Link(FStateTreeLinker& Linker)
{
	return FMassStateTreeTaskBase::Link(Linker);
}

EStateTreeRunStatus FMW_FindPlayerLocationTask::EnterState(FStateTreeExecutionContext& Context,
	const FStateTreeTransitionResult& Transition) const
{
	if (Transition.ChangeType != EStateTreeStateChangeType::Changed) { return EStateTreeRunStatus::Running; }
	TRACE_CPUPROFILER_EVENT_SCOPE(ST_GetPlayerPosition)
	
	FInstanceDataType& InstanceData = Context.GetInstanceData(*this);

	// Get the PlayerTrackingSubsystem
	UWorld* World = Context.GetWorld();
	if (!World)
	{
		return EStateTreeRunStatus::Failed;
	}

	// Get the First Player Controller
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController)
	{
		return EStateTreeRunStatus::Failed;
	}

	// Get the Controlled Pawn (Player Character)
	APawn* PlayerPawn = PlayerController->GetPawn();
	if (!PlayerPawn)
	{
		return EStateTreeRunStatus::Failed;
	}

	// Get the Player's Position
	InstanceData.OutLocation = PlayerPawn->GetActorLocation();
	UE_LOG(LogTemp, Warning, TEXT("Player Position: %s"), *InstanceData.OutLocation.ToString());
	
	return EStateTreeRunStatus::Running;
}
