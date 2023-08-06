// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameFlowStateComponent.h"
#include "GameplayTagContainer.h"
#include "GameFramework/GameState.h"
#include "GameFlowAction.generated.h"

DECLARE_DELEGATE_TwoParams(FGameFlowActionFinishedDelegate, class UGameFlowAction* Action, bool bInterrupted);

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class GAMEFLOWPLUGIN_API UGameFlowAction : public UObject
{
	GENERATED_BODY()

public:
	UGameFlowAction(const FObjectInitializer& ObjectInitializer);
	UGameFlowAction();

	virtual UWorld* GetWorld() const override { return GetOuter()->GetOuter()->GetWorld(); }

	FGameFlowActionFinishedDelegate OnFinished;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	bool bTickable = false;

	// The higher the priority, the earlier the action will be ticked relative to other tickable actions.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow", meta = (EditCondition = "bTickable"))
	int32 TickPriority = 0;

	// The tag representing the chain this action is a part of if the State is executing in a branched fashion.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	FGameplayTag BranchedChainTag;

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	UGameFlowStateComponent* GetGameFlowStateComponent() const;

	void StartAction();
	UFUNCTION(BlueprintNativeEvent, Category = "GameFlow")
	void OnActionStart();

	void TickAction(float DeltaTime);
	UFUNCTION(BlueprintNativeEvent, Category = "GameFlow")
	void OnActionTick(float DeltaTime);

	void EndAction();
	UFUNCTION(BlueprintNativeEvent, Category = "GameFlow")
	void OnActionEnd();

	void InterruptActionNative();
	UFUNCTION(BlueprintNativeEvent, Category = "GameFlow")
	void OnActionInterrupted();

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void InterruptAction();

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	void FinishAction();

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	bool IsTickable() const { return bTickable; }

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	int32 GetTickPriority() const { return TickPriority; }

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	bool IsEnabled() const { return bEnabled; }

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	UGameFlowStateComponent* GetStateComponent() const { return StateComponent.Get(); }

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	AGameStateBase* GetGameState() const { return GetStateComponent()->GetTypedOuter<AGameStateBase>(); }

	template<typename T>
	T* GetGameState() const { return Cast<T>(GetGameState()); }

	UPROPERTY()
	bool bEnabled = false;
	
	UPROPERTY()
	TObjectPtr<UGameFlowStateComponent> StateComponent;
};
