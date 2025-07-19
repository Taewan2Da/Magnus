// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EREnemyStatComponent.generated.h"

//Magnus

DECLARE_MULTICAST_DELEGATE(M_FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(M_FOnHpChangeDelegate, float /*CurrentHp*/);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class ETERNALRETUURN_API UEREnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEREnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	M_FOnHpZeroDelegate M_OnHpZero;
	M_FOnHpChangeDelegate M_OnHpChanged;

	FORCEINLINE float GetMaxHp() { return M_MaxHp; }
	FORCEINLINE float GetCurrentHp() { return M_CurrentHp; }
	float M_ApplyDamage(float InDamage);

protected:
	void M_SetHp(float M_newHp);

	UPROPERTY(VisibleInstanceOnly, Category = Stat)
	float M_MaxHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float M_CurrentHp;
		
};
