// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ERUserWidget.h"
#include "ERHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALRETUURN_API UERHpBarWidget : public UERUserWidget
{
	GENERATED_BODY()

public:
	UERHpBarWidget(const FObjectInitializer& ObjectInitializer);

	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	virtual void NativeConstruct() override;

	
protected:
	UPROPERTY()
	TObjectPtr<class UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
	
};
