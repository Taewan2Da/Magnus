// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ERCharacterAIInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UERCharacterAIInterface : public UInterface
{
	GENERATED_BODY()
};

DECLARE_DELEGATE(FAICharacterAttackFinished);
/**
 * 
 */
class ETERNALRETUURN_API IERCharacterAIInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetAIPatrolRadius() = 0;
	virtual float GetAIDetectRange() = 0;
	virtual float GetAIAttackRange() = 0;
	virtual float GetAITurnSpeed() = 0;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) = 0;
	virtual void AttackByAI() = 0;

};
