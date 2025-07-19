// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "M_UserWidget.h"
#include "MagnusHpBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALRETUURN_API UMagnusHpBarWidget : public UM_UserWidget
{
	GENERATED_BODY()
	
public:
	UMagnusHpBarWidget(const FObjectInitializer& ObjectInitializer);

public:

	FORCEINLINE void M_SetMaxHp(float M_NewMaxHp) { M_MaxHp = M_NewMaxHp; }
	void M_UpdateHpBar(float M_NewCurrentHp);
	void M_UpdateCooldownBar(float M_Cooldown);

	// [�߰�] ��ٿ� ���� �Լ�
	UFUNCTION(BlueprintCallable)
	void StartCooldown(float InCooldownTime);

protected:
	virtual void NativeConstruct() override;

public:
	void InitWidgetLogic();

protected:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> M_HpProgressBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UProgressBar> Cooldown;

	UPROPERTY()
	float CooldownPercent;

	UPROPERTY()
	float M_MaxHp;

	// [�߰�] ��ٿ� Ÿ�̸� ���� ����
	FTimerHandle CooldownTimerHandle;
	float CurrentCooldownTime;
	float MaxCooldownTime;

	// [�߰�] ��ٿ� Tick �Լ�
	void CooldownTick();

};
