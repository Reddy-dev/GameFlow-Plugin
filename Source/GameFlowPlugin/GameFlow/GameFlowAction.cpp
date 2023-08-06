// Copyright Solstice Games

#include "GameFlowAction.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFlowAction)

UGameFlowAction::UGameFlowAction(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}

UGameFlowAction::UGameFlowAction()
{
	
}

UGameFlowStateComponent* UGameFlowAction::GetGameFlowStateComponent() const
{
	return StateComponent.Get();
}

void UGameFlowAction::StartAction()
{
	bEnabled = true;
}

void UGameFlowAction::TickAction(float DeltaTime)
{
	if (bEnabled && bTickable)
	{
		OnActionTick(DeltaTime);
	}
}

void UGameFlowAction::EndAction()
{
	if (bEnabled)
	{
		bEnabled = false;
		OnFinished.Execute(this, false);
		OnActionEnd();
	}
}

void UGameFlowAction::InterruptActionNative()
{
	if (bEnabled)
	{
		bEnabled = false;
		OnFinished.ExecuteIfBound(this, true);
		OnActionInterrupted();
	}
}

void UGameFlowAction::InterruptAction()
{
	
}

void UGameFlowAction::FinishAction()
{
	
}

void UGameFlowAction::OnActionInterrupted_Implementation()
{
	
}

void UGameFlowAction::OnActionEnd_Implementation()
{
	
}

void UGameFlowAction::OnActionTick_Implementation(float DeltaTime)
{
	
}

void UGameFlowAction::OnActionStart_Implementation()
{
	
}
