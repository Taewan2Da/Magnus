// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ERUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALRETUURN_API UERUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	FORCEINLINE void SetOwningActor(AActor* NewOnwer) { OwningActor = NewOnwer; }


protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Actor")
	TObjectPtr<AActor> OwningActor;

};
