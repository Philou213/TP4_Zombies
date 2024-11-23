// Fill out your copyright notice in the Description page of Project Settings.


#include "Team.h"

// Sets default values for this component's properties
UTeam::UTeam()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

bool UTeam::IsSameTeam(int otherTeamId)
{
	return teamId == otherTeamId;
}

int UTeam::GetTeamId()
{
	return teamId;
}



