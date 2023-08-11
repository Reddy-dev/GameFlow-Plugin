// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameFlowPlugin/GameFlow/GameFlowAction.h"
#include "GameFlowAction_StartState.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Start State"), EditInlineNew, DefaultToInstanced)
class GAMEFLOWPLUGIN_API UGameFlowAction_StartState : public UGameFlowAction
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow",
		meta = (EditCondition = "StateType == EGameFlowStateType::GameplayTag"))
	FGameplayTag StateTag;

	virtual void OnActionStart_Implementation() override;
};
