#include "BTTask_Fire.h"
#include "AIController.h"
#include "TP3Shoot/TP3ShootCharacter.h"

UBTTask_Fire::UBTTask_Fire()
{
    NodeName = "Fire Weapon";
    CooldownTime = 0.075f;  // Set your desired cooldown time (in seconds)
    LastFireTime = -CooldownTime;  // Ensure it can fire immediately at the start
}

EBTNodeResult::Type UBTTask_Fire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    // Get the AI controller and controlled pawn
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr) return EBTNodeResult::Failed;

    // Cast to your character class
    ATP3ShootCharacter* Character = Cast<ATP3ShootCharacter>(AIController->GetPawn());
    if (Character == nullptr) return EBTNodeResult::Failed;

    // Check if enough time has passed since the last shot (cooldown)
    float CurrentTime = AIController->GetWorld()->GetTimeSeconds();
    if (CurrentTime - LastFireTime > CooldownTime)
    {
        // Call the Fire function
        Character->FireStraight();

        // Update the last fire time
        LastFireTime = CurrentTime;

        // Return success after firing
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}
