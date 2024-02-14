// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameFlowPlugin/GameFlow/GameFlowAction.h"
#include "GameFlowAction_FinishState.generated.h"

UCLASS(BlueprintType, Blueprintable, meta = (DisplayName = "Finish State"), EditInlineNew, DefaultToInstanced)
class GAMEFLOWPLUGIN_API UGameFlowAction_FinishState : public UGameFlowAction
{
	GENERATED_BODY()

public:

	virtual void OnActionStart_Implementation() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow")
	bool bFinishOwningState = true;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "GameFlow", meta = (EditCondition = "!bFinishOwningState",
		EditConditionHides))
	FGameplayTag StateTag;
}; // class UGameFlowAction_FinishState
