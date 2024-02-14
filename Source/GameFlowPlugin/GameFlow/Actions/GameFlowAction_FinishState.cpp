// Copyright Solstice Games

#include "GameFlowAction_FinishState.h"
#include "GameFlowPlugin/GameFlow/GameFlowState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFlowAction_FinishState)

void UGameFlowAction_FinishState::OnActionStart_Implementation()
{
	if (bFinishOwningState)
	{
		// ReSharper disable once CppTooWideScopeInitStatement
		UGameFlowState* OwningState = GetTypedOuter<UGameFlowState>();

		if (ensureMsgf(OwningState, TEXT("GameFlowAction_FinishState: Failed to finish owning state because it is not a UGameFlowState or it isn't valid")))
		{
			OwningState->FinishState();
			FinishAction(true);
		}

		InterruptActionNative();
		return;
	}

	if (!ensureMsgf(StateTag.IsValid(), TEXT("GameFlowAction_FinishState: Failed to finish state because StateTag is invalid")))
	{
		InterruptActionNative();
		return;
	}
	
	if (UGameFlowState* State = GetStateComponent()->GetStateByTag(StateTag))
	{
		State->FinishState();
		FinishAction(false);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("GameFlowAction_FinishState: Failed to finish state with tag %s"), *StateTag.ToString());
		InterruptActionNative();
	}
}