// Copyright Solstice Games

#include "GameFlowState.h"
#include "GameFlowAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFlowState)

UGameFlowState::UGameFlowState(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

UGameFlowState::UGameFlowState()
{
	
}

void UGameFlowState::StartState(UGameFlowStateComponent* GameFlowComponent)
{
	checkf(GameFlowComponent, TEXT("GameFlowComponent is null!"));
	GameStateFlowComponent = GameFlowComponent;
	for (UGameFlowAction* Action : Actions)
	{
		Action->StateComponent = GameFlowComponent;
	}
	
	switch (ExecutionMode)
	{
		case EGameFlowExecutionMode::Linear:
			Actions[0]->OnFinished.BindUObject(this, &UGameFlowState::OnStateFinished);
			Actions[0]->StartAction();
			break;
		case EGameFlowExecutionMode::Parallel:
			for (UGameFlowAction* Action : Actions)
			{
				Action->OnFinished.BindUObject(this, &UGameFlowState::OnStateFinished);
				Action->StartAction();
			}
			break;
		/* The game flow will be executed in a branched fashion, meaning that each node will be
		executed within a chain, but each chain will execute simultaneously. */
		case EGameFlowExecutionMode::Branched:
			ChainActions.Empty();
			for (UGameFlowAction* Action : Actions)
			{
				if (Action->BranchedChainTag.IsValid())
				{
					ChainActions.FindOrAdd(Action->BranchedChainTag).AddUnique(Action);
				}
			}

			for (auto& Chain : ChainActions)
			{
				Chain.Value[0]->OnFinished.BindUObject(this, &UGameFlowState::OnStateFinished);
				Chain.Value[0]->StartAction();
			}
			break;
		default: ;
	};
}

void UGameFlowState::FinishState()
{
	for (UGameFlowAction* Action : Actions)
	{
		Action->OnFinished.Unbind();
		
		if (Action->bEnabled)
		{
			Action->FinishAction();
		}
		
		Action->bEnabled = false;
	}
	
	OnStateFinishedDelegate.Broadcast();
}

void UGameFlowState::InterruptState()
{
	for (UGameFlowAction* Action : Actions)
	{
		Action->OnFinished.Unbind();
		ensureMsgf(!Action->bEnabled, TEXT("Action is still enabled!"));
		Action->InterruptActionNative();
	}
}

void UGameFlowState::OnStateFinished(UGameFlowAction* InAction, bool bInterrupted)
{
	int32 Index = -1;
	InAction->OnFinished.Unbind();
	
	if (bInterrupted && bCancelOnInterrupt)
	{
		FinishState();
		return;
	}

	if (ExecutionMode == EGameFlowExecutionMode::Linear)
	{
		Index = Actions.Find(InAction);
		if (Index != INDEX_NONE && Index + 1 < Actions.Num())
		{
			Actions[Index + 1]->OnFinished.BindUObject(this, &UGameFlowState::OnStateFinished);
			Actions[Index + 1]->StartAction();
		}
		else
		{
			FinishState();
		}
	}
	else if (ExecutionMode == EGameFlowExecutionMode::Parallel)
	{
		for (const UGameFlowAction* Action : Actions)
		{
			if (Action->bEnabled)
			{
				return;
			}
		}

		FinishState();
	}
	else if (ExecutionMode == EGameFlowExecutionMode::Branched)
	{
		TArray<UGameFlowAction*> Chain;
		checkf(ChainActions.Contains(InAction->BranchedChainTag), TEXT("Chain tag not found!"));
		TArray<UGameFlowAction*>& LocChainActions = ChainActions[InAction->BranchedChainTag];
		Index = LocChainActions.Find(InAction);
		if (Index != INDEX_NONE && Index + 1 < LocChainActions.Num())
		{
			LocChainActions[Index + 1]->OnFinished.BindUObject(this, &UGameFlowState::OnStateFinished);
			LocChainActions[Index + 1]->StartAction();
		}
		else
		{
			ChainActions.Remove(InAction->BranchedChainTag);
			if (ChainActions.Num() == 0)
			{
				FinishState();
			}
		}
	}
}
