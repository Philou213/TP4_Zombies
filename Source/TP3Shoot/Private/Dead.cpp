// Fill out your copyright notice in the Description page of Project Settings.


#include "Dead.h"

#include "Health.h"
#include "Respawn.h"
#include "Team.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UDead::UDead()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UDead::BeginPlay()
{
	Super::BeginPlay();

	UHealth* Health = GetOwner()->FindComponentByClass<UHealth>();
	if (Health)
	{
		Health->OnDead.AddDynamic(this, &UDead::DeactivateCharacter);
	}

	TArray<AActor*> FoundRespawns;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARespawn::StaticClass(), FoundRespawns);

	for (AActor* FoundRespawnActor : FoundRespawns)
	{	
		ARespawn* FoundRespawn = Cast<ARespawn>(FoundRespawnActor);
		UTeam* RespawnTeam = FoundRespawn->GetComponentByClass<UTeam>();

		UTeam* CharacterTeam = GetOwner()->FindComponentByClass<UTeam>();

		if (CharacterTeam->IsSameTeam(RespawnTeam->teamId)) Respawn = FoundRespawn;
	}
}

void UDead::DeactivateCharacter()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		ACharacter* Character = Cast<ACharacter>(OwnerActor);

		if (Character)
		{
			UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
			if (MovementComponent)
			{
				MovementComponent->SetMovementMode(MOVE_None);
			}
		}
		OwnerActor->SetActorEnableCollision(false);
		OwnerActor->SetActorHiddenInGame(true);
		OwnerActor->SetActorTickEnabled(false);
	}
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &UDead::ReactivateCharacter, RespawnCooldown);
}

void UDead::ReactivateCharacter()
{
	AActor* OwnerActor = GetOwner();
	if (OwnerActor)
	{
		ACharacter* Character = Cast<ACharacter>(OwnerActor);

		if (Character)
		{
			UCharacterMovementComponent* MovementComponent = Character->GetCharacterMovement();
			if (MovementComponent)
			{
				MovementComponent->SetMovementMode(MOVE_Walking);
			}
		}

		if (Respawn == nullptr) OwnerActor->SetActorLocationAndRotation(FVector(1490,2010,350), FRotator::ZeroRotator);
		else OwnerActor->SetActorLocationAndRotation(Respawn->GetActorLocation(), Respawn->GetActorRotation());
		
		OwnerActor->SetActorEnableCollision(true);
		OwnerActor->SetActorHiddenInGame(false);
		OwnerActor->SetActorTickEnabled(true);
	}
}
