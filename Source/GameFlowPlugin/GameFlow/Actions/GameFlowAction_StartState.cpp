// Copyright Solstice Games

#include "GameFlowAction_StartState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFlowAction_StartState)

void UGameFlowAction_StartState::OnActionStart_Implementation()
{
	Super::OnActionStart_Implementation();

	if (GetStateComponent()->StartStateByTag(StateTag))
	{
		FinishAction(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameFlowAction_StartState: Failed to start state with tag %s"), *StateTag.ToString());
		InterruptActionNative();
	}
}
