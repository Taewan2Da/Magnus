// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyUI/M_WidgetComponent.h"
#include "M_UserWidget.h"

void UM_WidgetComponent::InitWidget()
{
	Super::InitWidget();

	UM_UserWidget* M_UserWidget = Cast<UM_UserWidget>(GetWidget());

	if (M_UserWidget)
	{
		M_UserWidget->M_SetOwningActor(GetOwner());
	}

}
