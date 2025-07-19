// Fill out your copyright notice in the Description page of Project Settings.


#include "MagnusHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Interface/M_WidgetInterface.h"

UMagnusHpBarWidget::UMagnusHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	M_MaxHp = -1.0f;
	CooldownPercent = 10.0f;

	CurrentCooldownTime = 0.0f;
	MaxCooldownTime = 1.0f;
}

void UMagnusHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ensure(M_HpProgressBar);
	M_HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("M_HpProgressBar")));


	IM_WidgetInterface* M_CharacterWidget = Cast<IM_WidgetInterface>(M_OwningActor);
	if (M_CharacterWidget)
	{
		M_CharacterWidget->M_SetupCharacterWidget(this);
	}

	ensure(Cooldown);
	Cooldown = Cast<UProgressBar>(GetWidgetFromName(TEXT("Cooldown")));

	if (Cooldown)
	{
		Cooldown->SetPercent(0.0f);
	}

	IM_WidgetInterface* CooldownWidget = Cast<IM_WidgetInterface>(M_OwningActor);
	if (CooldownWidget)
	{
		CooldownWidget->M_SetupCharacterWidget(this);
	}

}

void UMagnusHpBarWidget::InitWidgetLogic()
{
	IM_WidgetInterface* M_CharacterWidget = Cast<IM_WidgetInterface>(M_OwningActor);
	if (M_CharacterWidget)
	{
		M_CharacterWidget->M_SetupCharacterWidget(this);
	}
}

void UMagnusHpBarWidget::CooldownTick()
{
	CurrentCooldownTime -= 0.02f;
	float Percent = FMath::Clamp(CurrentCooldownTime / MaxCooldownTime, 0.0f, 1.0f);

	if (Cooldown)
	{
		Cooldown->SetPercent(Percent);
	}

	if (CurrentCooldownTime <= 0.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(CooldownTimerHandle);
		if (Cooldown)
		{
			Cooldown->SetPercent(0.0f); // 쿨다운 끝나면 0으로!
		}
	}
}

void UMagnusHpBarWidget::M_UpdateHpBar(float M_NewCurrentHp)
{
	ensure(M_MaxHp > 0.0f);
	if (M_HpProgressBar)
	{
		M_HpProgressBar->SetPercent(M_NewCurrentHp / M_MaxHp);
	}
}

void UMagnusHpBarWidget::M_UpdateCooldownBar(float M_Cooldown)
{
	ensure(M_Cooldown < 10.0f);
	{
		if (Cooldown)
		{
			Cooldown->SetPercent(M_Cooldown / CooldownPercent);
		}
	}
}

void UMagnusHpBarWidget::StartCooldown(float InCooldownTime)
{
	MaxCooldownTime = InCooldownTime;
	CurrentCooldownTime = MaxCooldownTime;

	if (Cooldown)
	{
		Cooldown->SetPercent(1.0f); // 쿨다운 시작 시 1로 설정
	}

	// 0.02초마다 CooldownTick 호출
	GetWorld()->GetTimerManager().SetTimer(
		CooldownTimerHandle, this, &UMagnusHpBarWidget::CooldownTick, 0.02f, true
	);
}
