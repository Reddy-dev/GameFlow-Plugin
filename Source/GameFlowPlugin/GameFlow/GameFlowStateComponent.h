// Copyright Solstice Games

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "GameFlowStateComponent.generated.h"

class UGameFlowAction;
class UGameFlowState;

// Place on Game State
UCLASS(ClassGroup=(GameFlow), meta=(BlueprintSpawnableComponent))
class GAMEFLOWPLUGIN_API UGameFlowStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UGameFlowStateComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	// Run at the Start of the Component's lifetime, aren't tied to any State
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GameFlow")
	TArray<TObjectPtr<UGameFlowAction>> SingletonActions;

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	UGameFlowAction* GetSingletonAction(TSubclassOf<UGameFlowAction> ActionClass);

	template <class T>
	T* GetSingletonAction()
	{
		return Cast<T>(GetSingletonAction(T::StaticClass()));
	}

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	bool StartStateByClass(TSubclassOf<UGameFlowState> StateClass);

	template <class T>
	bool StartStateByClass()
	{
		return StartStateByClass(T::StaticClass());
	}

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	bool StartStateByTag(FGameplayTag StateTag);

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	bool StartState(UGameFlowState* State);

	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	UGameFlowState* GetStateByClass(TSubclassOf<UGameFlowState> StateClass) const;

	template <class T>
	T* GetStateByClass()
	{
		return Cast<T>(GetStateByClass(T::StaticClass()));
	}
	
	UFUNCTION(BlueprintCallable, Category = "GameFlow")
	UGameFlowState* GetStateByTag(FGameplayTag StateTag) const;
	
	UPROPERTY(EditAnywhere, Instanced, BlueprintReadOnly, Category = "GameFlow")
	TMap<FGameplayTag, TObjectPtr<UGameFlowState>> StateMap;

	void AddTickableActions(const TArray<TWeakObjectPtr<UGameFlowAction>>& Actions);
	void RemoveTickableActions(const TArray<TWeakObjectPtr<UGameFlowAction>>& Actions);
	void SortTickableActions();

	UPROPERTY()
	TArray<TWeakObjectPtr<UGameFlowAction>> TickableActions;
};
