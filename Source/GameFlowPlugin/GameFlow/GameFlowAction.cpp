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

void UGameFlowAction::EndAction(const bool bInterrupted)
{
	if (LIKELY(bEnabled))
	{
		bEnabled = false;
		OnFinished.Execute(this, false);
		OnActionEnd(bInterrupted);
	}
}

void UGameFlowAction::InterruptActionNative()
{
	if (LIKELY(bEnabled))
	{
		OnActionInterrupted();
		FinishAction(true);
	}
}

void UGameFlowAction::InterruptAction()
{
	InterruptActionNative();
}

void UGameFlowAction::FinishAction(const bool bInterrupted)
{
	EndAction(bInterrupted);
}

void UGameFlowAction::OnActionInterrupted_Implementation()
{
	
}

void UGameFlowAction::OnActionEnd_Implementation(const bool bInterrupted)
{
	
}

void UGameFlowAction::OnActionTick_Implementation(float DeltaTime)
{
	
}

void UGameFlowAction::OnActionStart_Implementation()
{
	
}
