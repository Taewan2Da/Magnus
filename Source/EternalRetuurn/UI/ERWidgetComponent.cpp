// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ERWidgetComponent.h"
#include "ERUserWidget.h"

void UERWidgetComponent::InitWidget()
{
	Super::InitWidget();

	UERUserWidget* ERUserWidget = Cast<UERUserWidget>(GetWidget());
	if (ERUserWidget)
	{
		ERUserWidget->SetOwningActor(GetOwner());
	}
}
