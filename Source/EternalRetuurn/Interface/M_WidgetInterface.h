// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "M_WidgetInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UM_WidgetInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ETERNALRETUURN_API IM_WidgetInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void M_SetupCharacterWidget(class UM_UserWidget* M_InUserWidget) = 0;
};
