// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameFlowExecutionMode.h"
#include "GameplayTagContainer.h"
#include "GameFlowState.generated.h"

class UGameFlowStateComponent;
class UGameFlowAction;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStateFinished);

UCLASS(EditInlineNew, BlueprintType, DefaultToInstanced)
class GAMEFLOWPLUGIN_API UGameFlowState : public UObject
{
	GENERATED_BODY()

public:
	UGameFlowState(const FObjectInitializer& ObjectInitializer);
	UGameFlowState();

	void StartState(UGameFlowStateComponent* GameFlowComponent);
	void FinishState();
	void InterruptState();

	FOnStateFinished OnStateFinishedDelegate;

	// Can't enter this state if any of these tags are not present on any of the states in the StateStack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	FGameplayTagContainer RequiredStateTags;

	// Can't enter this state if any of these tags are present on any of the states in the StateStack
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	FGameplayTagContainer ExcludedStateTags;

	// Finish any states in the StateStack that have any of these tags
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	FGameplayTagContainer FinishStateTags;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	EGameFlowExecutionMode ExecutionMode = EGameFlowExecutionMode::Linear;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	bool bCancelOnInterrupt = true;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GameFlow")
	TArray<TObjectPtr<UGameFlowAction>> Actions;

	TMap<FGameplayTag, TArray<UGameFlowAction*>> ChainActions;

	UFUNCTION()
	void OnStateFinished(UGameFlowAction* Action, bool bInterrupted);

protected:
	UPROPERTY()
	TObjectPtr<UGameFlowStateComponent> GameStateFlowComponent;
}; // class UGameFlowState
