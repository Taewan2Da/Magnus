// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "ERGameMode.generated.h"

/**
 * 
 */
UCLASS()
class ETERNALRETUURN_API AERGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AERGameMode();

	UPROPERTY(BlueprintReadOnly)
	int32 DeadEnemyCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> ClearWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> StartWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<UUserWidget> DeadWidgetClass;
	
	UFUNCTION(BlueprintCallable)
	void ShowClearWidget();
	
};
