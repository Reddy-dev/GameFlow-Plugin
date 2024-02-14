#pragma once

#include "CoreMinimal.h"
#include "GameFlowExecutionMode.generated.h"

UENUM(BlueprintType)
enum class EGameFlowExecutionMode : uint8
{
	/* The game flow will be executed in a linear fashion, meaning that the game flow
	 will be executed from the first node to the last node with each node waiting for
	 the previous node to finish before executing. */
	Linear = 0,
	/* The game flow will be executed in a parallel fashion, meaning that the game flow
	 will be executed from the first node to the last node with each node executing
	 simultaneously. */
	Parallel = 1,
	/* The game flow will be executed in a branched fashion, meaning that each node will be
	 executed within a chain, but each chain will execute simultaneously. */
	Branched = 2
}; // enum class EGameFlowExecutionMode 