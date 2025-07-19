// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Animation/AnimMontage.h"
#include "ERComboActionData.generated.h"

USTRUCT(BlueprintType)
struct FAttackInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Range = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Radius = 60.0f;
};

UCLASS()
class ETERNALRETUURN_API UERComboActionData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UERComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = Name)
	TArray<float>  EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category = Attack)
	TMap<UAnimMontage*, FAttackInfo> MontageAttackInfoMap;
	
};
