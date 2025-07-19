// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "M_UserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALRETUURN_API UM_UserWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	FORCEINLINE void M_SetOwningActor(AActor* NewOnwer) 
	{
		UE_LOG(LogTemp, Warning, TEXT("M_SetOwningActor »£√‚! NewOwner: %s"), *GetNameSafe(NewOnwer));
		M_OwningActor = NewOnwer; 
	}


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor")
	TObjectPtr<AActor> M_OwningActor;

	
};
