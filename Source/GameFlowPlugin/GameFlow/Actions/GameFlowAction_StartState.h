// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameFlowPlugin/GameFlow/GameFlowAction.h"
#include "GameFlowAction_StartState.generated.h"

UENUM(BlueprintType)
enum class EGameFlowStateType : uint8
{
	GameplayTag,
	SubclassOf,
};

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Start State"), EditInlineNew, DefaultToInstanced)
class GAMEFLOWPLUGIN_API UGameFlowAction_StartState : public UGameFlowAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	EGameFlowStateType StateType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow",
		meta = (EditCondition = "StateType == EGameFlowStateType::GameplayTag"))
	FGameplayTag StateTag;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow",
		meta = (EditCondition = "StateType == EGameFlowStateType::SubclassOf"))
	TSubclassOf<UGameFlowState> StateClass;

	virtual void OnActionStart_Implementation() override;
};
