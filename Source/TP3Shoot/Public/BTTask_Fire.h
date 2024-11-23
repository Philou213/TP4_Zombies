#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Fire.generated.h"

/**
 *
 */
UCLASS()
class TP3SHOOT_API UBTTask_Fire : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_Fire();

protected:
	// Override the ExecuteTask function to perform the behavior
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	// Cooldown time between shots (in seconds)
	UPROPERTY(EditAnywhere, Category = "Fire Settings")
	float CooldownTime;

	// Time when the last shot was fired
	float LastFireTime;
};
