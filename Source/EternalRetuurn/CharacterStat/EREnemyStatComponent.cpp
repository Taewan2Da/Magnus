// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/EREnemyStatComponent.h"

// Sets default values for this component's properties
UEREnemyStatComponent::UEREnemyStatComponent()
{
	M_MaxHp = 500.0f;
	M_CurrentHp = M_MaxHp;
}


// Called when the game starts
void UEREnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();

	M_SetHp(M_MaxHp);
	
}

float UEREnemyStatComponent::M_ApplyDamage(float InDamage)
{
	const float PrevHp = M_CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	M_SetHp(PrevHp - ActualDamage);
	if (M_CurrentHp <= KINDA_SMALL_NUMBER)
	{
		M_OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UEREnemyStatComponent::M_SetHp(float M_newHp)
{
	M_CurrentHp = FMath::Clamp<float>(M_newHp, 0.0f, M_MaxHp);

	M_OnHpChanged.Broadcast(M_CurrentHp);
}




