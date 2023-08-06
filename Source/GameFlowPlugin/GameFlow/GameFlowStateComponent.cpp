// Copyright Solstice Games

#include "GameFlowStateComponent.h"
#include "GameFlowAction.h"
#include "GameFlowState.h"
#include "Net/UnrealNetwork.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFlowStateComponent)

UGameFlowStateComponent::UGameFlowStateComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	PrimaryComponentTick.TickGroup = TG_PrePhysics;
}

void UGameFlowStateComponent::BeginPlay()
{
	Super::BeginPlay();

	checkf(GetTypedOuter<AGameStateBase>() != nullptr, TEXT("GameFlowStateComponent must be attached to a GameState"));

	for (UGameFlowAction* Singleton : SingletonActions)
	{
		if (ensureMsgf(IsValid(Singleton), TEXT("Invalid singleton action")))
		{
			Singleton->StateComponent = this;
			if (Singleton->bTickable)
			{
				TickableActions.AddUnique(Singleton);
			}
			
			Singleton->StartAction();
		}
	}

	SortTickableActions();
}

void UGameFlowStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	FDoRepLifetimeParams SharedParams;
	SharedParams.bIsPushBased = true;

	
}

void UGameFlowStateComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for (int32 i = TickableActions.Num() - 1; i >= 0; --i)
	{
		TWeakObjectPtr<UGameFlowAction> Action = TickableActions[i];
		if (Action.IsValid() && Action->bEnabled)
		{
			Action->TickAction(DeltaTime);
		}
		else
		{
			TickableActions.RemoveAt(i);
		}
	}
}

UGameFlowAction* UGameFlowStateComponent::GetSingletonAction(TSubclassOf<UGameFlowAction> ActionClass)
{
	checkf(ActionClass, TEXT("Invalid action class"));
	
	for (UGameFlowAction* Action : SingletonActions)
	{
		if (Action->GetClass() == ActionClass)
		{
			return Action;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("Singleton action %s not found"), *ActionClass->GetName());
	return nullptr;
}

bool UGameFlowStateComponent::StartStateByClass(const TSubclassOf<UGameFlowState> StateClass)
{
	for (const TTuple<FGameplayTag, TObjectPtr<UGameFlowState>>& StatePair : StateMap)
	{
		if (StatePair.Value->GetClass() == StateClass)
		{
			return StartState(StatePair.Value.Get());
		}
	}

	return false;
}

bool UGameFlowStateComponent::StartStateByTag(const FGameplayTag StateTag)
{
	if (ensureMsgf(StateMap.Contains(StateTag), TEXT("State tag not found!")))
	{
		return StartState(StateMap[StateTag].Get());
	}

	return false; 
}

bool UGameFlowStateComponent::StartState(UGameFlowState* State)
{
	if (ensureMsgf(State, TEXT("Invalid state!")))
	{
		const FGameplayTagContainer ExcludedStateTags = State->ExcludedStateTags;
		const FGameplayTagContainer RequiredStateTags = State->RequiredStateTags;
		const FGameplayTagContainer FinishStateTags = State->FinishStateTags;

		for (TTuple<FGameplayTag, TObjectPtr<UGameFlowState>>& StatePair : StateMap)
		{
			if (RequiredStateTags.HasTagExact(StatePair.Key))
			{
				return false;
			}

			if (ExcludedStateTags.HasTagExact(StatePair.Key))
			{
				return false;
			}

			if (FinishStateTags.HasTagExact(StatePair.Key))
			{
				StatePair.Value->InterruptState();
			}
		}

		State->StartState(this);
	}

	return false;
}

UGameFlowState* UGameFlowStateComponent::GetStateByClass(TSubclassOf<UGameFlowState> StateClass) const
{
	for (const TTuple<FGameplayTag, TObjectPtr<UGameFlowState>>& StatePair : StateMap)
	{
		if (StatePair.Value->GetClass() == StateClass)
		{
			return StatePair.Value.Get();
		}
	}
	
	return nullptr;
}

UGameFlowState* UGameFlowStateComponent::GetStateByTag(FGameplayTag StateTag) const
{
	if (ensureMsgf(StateMap.Contains(StateTag), TEXT("State tag not found!")))
	{
		return StateMap[StateTag].Get();
	}

	return nullptr;
}

void UGameFlowStateComponent::AddTickableActions(const TArray<TWeakObjectPtr<UGameFlowAction>>& Actions)
{
	TickableActions.Append(Actions);
	SortTickableActions();
}

void UGameFlowStateComponent::RemoveTickableActions(const TArray<TWeakObjectPtr<UGameFlowAction>>& Actions)
{
	TickableActions.RemoveAll([&](const TWeakObjectPtr<UGameFlowAction>& Action)
	{
		return Actions.Contains(Action);
	});

	SortTickableActions();
}

void UGameFlowStateComponent::SortTickableActions()
{
	// Sort by priority in ascending order
	TickableActions.Sort([](const TWeakObjectPtr<UGameFlowAction>& A, const TWeakObjectPtr<UGameFlowAction>& B)
	{
		return A->GetTickPriority() < B->GetTickPriority();
	});
}
