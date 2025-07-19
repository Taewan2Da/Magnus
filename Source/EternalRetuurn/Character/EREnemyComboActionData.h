// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EREnemyComboActionData.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALRETUURN_API UEREnemyComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UEREnemyComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float>  EffectiveFrameCount;
	
	//초 변수 만들기 1초
};
