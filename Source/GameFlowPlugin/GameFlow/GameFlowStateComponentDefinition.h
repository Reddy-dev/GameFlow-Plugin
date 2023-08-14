// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "GameFlowStateComponentDefinition.generated.h"

class UGameFlowStateComponent;
class UGameFlowState;
class UGameFlowAction;

UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class GAMEFLOWPLUGIN_API UGameFlowStateComponentDefinition : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	virtual FPrimaryAssetId GetPrimaryAssetId() const override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Instanced, Category = "GameFlow")
	TArray<TObjectPtr<UGameFlowAction>> SingletonActions;

	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GameFlow")
	TMap<FGameplayTag, TObjectPtr<UGameFlowState>> StateMap;
};
