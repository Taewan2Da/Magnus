// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Attack.h"
#include "AIController.h"
#include "Interface/ERCharacterAIInterface.h"

UBTTask_Attack::UBTTask_Attack()
{
}

EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* ControllingPawn = Cast<APawn>(OwnerComp.GetAIOwner()->GetPawn());
    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }

    //여기 문제임 IERCharacterAIInterface
    IERCharacterAIInterface* AIPawn = Cast<IERCharacterAIInterface>(ControllingPawn);
    if (nullptr == AIPawn)
    {
        return EBTNodeResult::Failed;
    }

    FAICharacterAttackFinished OnAttackFinished;
    OnAttackFinished.BindLambda(
        [&]()
        {
            FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
        }
    );

    AIPawn->SetAIAttackDelegate(OnAttackFinished);
    AIPawn->AttackByAI();
    return EBTNodeResult::InProgress;

}
