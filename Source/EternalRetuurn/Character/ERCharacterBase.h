// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interface/ERAnimationAttackinterface.h"
#include "Interface/M_WidgetInterface.h"
#include "ERCharacterBase.generated.h"

UCLASS()
class ETERNALRETUURN_API AERCharacterBase : public ACharacter, public IERAnimationAttackInterface, public IM_WidgetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AERCharacterBase();

	virtual void PostInitializeComponents() override;

	// ComboAction Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr <class UAnimMontage> ComboActionMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr <class UAnimMontage> ComboActionMontage2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UERComboActionData> ComboActionData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attack, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr <class UERComboActionData> ComboActionData2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr <class UAnimMontage> EmotionAsset1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr <class UAnimMontage> EmotionAsset2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr <class UAnimMontage> SkillAsset1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr <class UAnimMontage> RollAsset;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Weapon")
	TObjectPtr<USkeletalMeshComponent> Weapon;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	bool bIsRolling = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float WalkSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float SprintSpeed = 900.0f;

	//���� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float MaxWalkSpeedInterpSpeed = 20.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float TargetMaxWalkSpeed = 500.0f;


	void StartSprint();
	void StopSprint();
	// Ÿ�̸� �ڵ�
	FTimerHandle WalkSpeedInterpTimerHandle;

	// ���� �Լ� ����
	void InterpMaxWalkSpeed();


	void ProcessComboCommand();

	void ComboActionBegin();
	void ComboActionEnd(class UAnimMontage* TargetMontage, bool IsProperlyEnded);


	void SetComboCheckTimer();
	void ComboCheck();

	void EmotionPlay();

	void SkillPlay();
	void RollPlay();

	void ResetSkillCooldown();
	void ResetRollCooldown();

	void OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	int32 CurrentCombo = 0;
	FTimerHandle ComboTimerHandle;
	bool HasNextComboCommand = false;

	// �б� Section
protected:

	// ��Ÿ�� �б�
	UPROPERTY()
	UAnimMontage* CurrentComboMontage;

	// �޺����� �б�
	UPROPERTY()
	TObjectPtr<class UERComboActionData> CurrentComboActionData = nullptr;

	// �̸�� �б�
	UPROPERTY()
	UAnimMontage* CurrentEmotion;

	// ��ų �б�
	UPROPERTY()
	UAnimMontage* CurrentSkill;

	float SkillCooldown = 10.0f;   // ��ų ��Ÿ��(��)
	float RollCooldown = 0.67f;    // ������ ��Ÿ��(��) // ������ ��� �ð���ŭ ��Ÿ��

	bool bCanUseSkill = true;
	bool bCanUseRoll = true;

	FTimerHandle SkillCooldownTimerHandle;
	FTimerHandle RollCooldownTimerHandle;

protected:
    bool bIsPlayingEmotion = false;
    FDelegateHandle EmotionMontageEndHandle;
    
    void StopEmotionMontage();
    void OnEmotionMontageEnded(UAnimMontage* Montage, bool bInterrupted);

	// ���º���
	UAnimMontage* PlayingEmotionMontage = nullptr;
	UAnimMontage* PlayingActionMontage = nullptr;

	// Attack Hit Section
protected:
	virtual void AttackHitCheck() override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;


	// Stat Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UEREnemyStatComponent> M_Stat;

	// UI Widget Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UMagnusHpBarWidget> M_HpBarWidget;

	virtual void M_SetupCharacterWidget(class UM_UserWidget* M_InUserWidget) override;

	// Dead Section
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class UAnimMontage> DeadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UUserWidget> M_DeadWidget;

	virtual void SetDead();
	void PlayDeadAnimation();
};
