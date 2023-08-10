// Copyright Solstice Games

#include "GameFlowAction_StartState.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFlowAction_StartState)

void UGameFlowAction_StartState::OnActionStart_Implementation()
{
	Super::OnActionStart_Implementation();

	switch (StateType)
	{
		case EGameFlowStateType::GameplayTag:
			GetStateComponent()->StartStateByTag(StateTag);
			break;
		case EGameFlowStateType::SubclassOf:
			GetStateComponent()->StartStateByClass(StateClass);
			break;
		default: ;
	}

	FinishAction();
}
