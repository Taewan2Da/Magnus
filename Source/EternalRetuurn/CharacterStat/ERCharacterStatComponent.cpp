// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/ERCharacterStatComponent.h"

// Sets default values for this component's properties
UERCharacterStatComponent::UERCharacterStatComponent()
{
	MaxHp = 1000.0f;
	CurrentHp = MaxHp;
}


// Called when the game starts
void UERCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	SetHp(MaxHp);
	
}


float UERCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}
	return ActualDamage;
}

void UERCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.0f, MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}

