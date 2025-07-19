// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ERCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "ERComboActionData.h"
#include "Physics/ERCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/EREnemyStatComponent.h"
#include "EnemyUI/M_WidgetComponent.h"
#include "EnemyUI/MagnusHpBarWidget.h"


// ���� ����
class AERCharacterPlayer;

// Sets default values
AERCharacterBase::AERCharacterBase()
{
	// Pawn�� ȸ���� �����ϱ� ���� �⺻���� ����
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 900.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	TargetMaxWalkSpeed = WalkSpeed;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	GetCharacterMovement()->GravityScale = 2.0f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));
	

	// Weapon
	FName WeaponSocket(TEXT("weapon_l"));
	Weapon = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPON"));
	Weapon->SetupAttachment(GetMesh(), TEXT("weapon_l"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SM_WEAPON(
		TEXT("/Game/Magnus/Blueprints/SM_road_sign_001_Skeleton.SM_road_sign_001_Skeleton"));
	if (SM_WEAPON.Succeeded())
	{
		Weapon->SetSkeletalMesh(SM_WEAPON.Object);
	}


	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(
		TEXT("/Script/Engine.SkeletalMesh'/Game/Magnus/Magnus.Magnus'"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(
		TEXT("/Game/Magnus/Animation/ABP_ERCharacter.ABP_ERCharacter_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Script/Engine.AnimMontage'/Game/Magnus/Animation/AM_Dead.AM_Dead'"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}



	// Stat Component
	M_Stat = CreateDefaultSubobject<UEREnemyStatComponent>(TEXT("M_Stat"));

	// Widget Component

	static ConstructorHelpers::FClassFinder<UMagnusHpBarWidget> M_HpBarWidgetRef(
		TEXT("/Game/Magnus/UI/WBP_Magnus.WBP_Magnus_C"));
	if (M_HpBarWidgetRef.Class)
	{
		M_HpBarWidget = CreateWidget<UMagnusHpBarWidget>(GetWorld(), M_HpBarWidgetRef.Class);
		if (M_HpBarWidget)
		{
			M_HpBarWidget->AddToViewport(); // ȭ�鿡 ����!
			M_HpBarWidget->M_SetOwningActor(this); // <-- ���⼭ ���� ����
			M_HpBarWidget->InitWidgetLogic();
		}
	}



	static ConstructorHelpers::FClassFinder<UUserWidget> M_DeadWidgetRef(
		TEXT("/Game/Magnus/UI/WBP_Dead.WBP_Dead_C"));
	if (M_DeadWidgetRef.Class)
	{
		M_DeadWidget = M_DeadWidgetRef.Class;
	}


}

void AERCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	M_Stat->M_OnHpZero.AddUObject(this, &AERCharacterBase::SetDead);
}

void AERCharacterBase::InterpMaxWalkSpeed()
{
	float Current = GetCharacterMovement()->MaxWalkSpeed;
	float NewSpeed = FMath::FInterpTo(Current, TargetMaxWalkSpeed, 0.01f, MaxWalkSpeedInterpSpeed);

	// ��ǥ���� ���� �����ϸ� ��Ȯ�� ���߰� Ÿ�̸� ����
	if (FMath::Abs(NewSpeed - TargetMaxWalkSpeed) < 1.f)
	{
		NewSpeed = TargetMaxWalkSpeed;
		GetWorld()->GetTimerManager().ClearTimer(WalkSpeedInterpTimerHandle);
	}

	GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
}

void AERCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}

	else
	{
		HasNextComboCommand = true;
	}
}

void AERCharacterBase::ComboActionBegin()
{
	//Combo Status
	CurrentCombo = 1;

	//Movement Setting �̰� ���� ������ �����̴� �ִϸ��̼Ƕ� ������ �Ȱ��� ���ڸ����� ����
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//Animation Setting
	const float AttackSpeedRate = 1.5f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CurrentComboMontage, AttackSpeedRate);

	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// ȸ�� ������ �Ҵ�
	PlayingActionMontage = CurrentComboMontage;


	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AERCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentComboMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();

}

void AERCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);

	CurrentCombo = 0;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
	GetWorldTimerManager().ClearTimer(ComboTimerHandle);
	if (TargetMontage == PlayingActionMontage)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		PlayingActionMontage = nullptr;
	}
}



void AERCharacterBase::SetComboCheckTimer()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, TEXT("ComboCheck"));
	if (CurrentComboActionData)
	{
		int32 ComboIndex = CurrentCombo - 1;
		ensure(CurrentComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

		const float AttackSpeedRate = 1.0f;
		float ComboEffectiveTime = (CurrentComboActionData->EffectiveFrameCount[ComboIndex] / CurrentComboActionData->FrameRate) / AttackSpeedRate;
		if (ComboEffectiveTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AERCharacterBase::ComboCheck, ComboEffectiveTime, false);
		}
	}

}


void AERCharacterBase::ComboCheck()
{
	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("ComboCheck"));
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, CurrentComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *CurrentComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, CurrentComboMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;
		
	}
}

void AERCharacterBase::EmotionPlay()
{
	if (bIsPlayingEmotion)
	{
		return;
		//StopEmotionMontage();
		
	}

	bIsPlayingEmotion = true;
	PlayingEmotionMontage = CurrentEmotion;

	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("EmotionPlay"));
	const float EmotionSpeedRate = 1.3f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CurrentEmotion, EmotionSpeedRate);

	// ���� �����
	if (Weapon)
	{
		Weapon->SetVisibility(false, true);
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Yellow, TEXT("Weapon Hidden!"));
	}
	else
	{
		//GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Red, TEXT("Weapon is nullptr!"));
	}

	// EndDelegate ���
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AERCharacterBase::OnEmotionMontageEnded);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentEmotion);
	
}

void AERCharacterBase::SkillPlay()
{
	if (!bCanUseSkill)
	{
		return;
	}

	bCanUseSkill = false;

	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("SkillPlay"));
	const float SkillSpeedRate = 1.3f;




	// ��ų �߿��� ī�޶� ���� ����
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	if (M_HpBarWidget)
	{
		M_HpBarWidget->StartCooldown(10.0f); // 10�� ��ٿ� ProgressBar
	}

	// ȸ�� ������ �Ҵ�
	PlayingActionMontage = CurrentSkill;

	// �� �߰�: AttackHitCheck���� �� ���� ��ų������ ����
	CurrentComboMontage = SkillAsset1;          // �Ǵ� CurrentSkill
	CurrentComboActionData = ComboActionData2;  // ��ų�� �����Ϳ���

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CurrentSkill, SkillSpeedRate);

	// ��Ÿ�� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(
		SkillCooldownTimerHandle,
		this,
		&AERCharacterBase::ResetSkillCooldown,
		SkillCooldown,
		false
	);

	// ��ų �ִϸ��̼� ������ ���� ����
	
	// 1. ��������Ʈ(�̺�Ʈ �ڵ鷯) ��ü ����
	FOnMontageEnded EndDelegate;
	// 2. �� ��������Ʈ�� "OnSkillMontageEnded" �Լ��� ���ε�
	EndDelegate.BindUObject(this, &AERCharacterBase::OnSkillMontageEnded);
	// 3. �ش� ��Ÿ��(CurrentSkill)�� ������ EndDelegate�� �����ϵ��� AnimInstance�� ���
	AnimInstance->Montage_SetEndDelegate(EndDelegate, CurrentSkill);

}

void AERCharacterBase::RollPlay()
{
	if(!bCanUseRoll)
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Yellow, TEXT("Roll Cooldown!"));
		return;
	}
	bCanUseRoll = false;

	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("RollPlay"));
	const float SkillSpeedRate = 1.4f;

	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RollAsset, SkillSpeedRate);

	// ��Ÿ�� Ÿ�̸� ����
	GetWorld()->GetTimerManager().SetTimer(
		RollCooldownTimerHandle,
		this,
		&AERCharacterBase::ResetRollCooldown,
		RollCooldown,
		false
	);
}

void AERCharacterBase::ResetSkillCooldown()
{
	bCanUseSkill = true;
}

void AERCharacterBase::ResetRollCooldown()
{
	bCanUseRoll = true;
}

void AERCharacterBase::OnSkillMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	if (Montage == PlayingActionMontage)
	{
		bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		PlayingActionMontage = nullptr;
	}
}

void AERCharacterBase::StartSprint()
{
	TargetMaxWalkSpeed = SprintSpeed; // ��: 900
    // Ÿ�̸Ӱ� �̹� ���� �ִٸ� �ߺ� ����
    if (!GetWorld()->GetTimerManager().IsTimerActive(WalkSpeedInterpTimerHandle))
    {
        GetWorld()->GetTimerManager().SetTimer(
            WalkSpeedInterpTimerHandle,
            this,
            &AERCharacterBase::InterpMaxWalkSpeed,
            0.01f, // 0.01��(10ms)���� ����
            true
        );
    }
}

void AERCharacterBase::StopSprint()
{
	TargetMaxWalkSpeed = WalkSpeed; // ��: 500
	// Ÿ�̸Ӱ� �̹� ���� �ִٸ� �ߺ� ����
	if (!GetWorld()->GetTimerManager().IsTimerActive(WalkSpeedInterpTimerHandle))
	{
		GetWorld()->GetTimerManager().SetTimer(
			WalkSpeedInterpTimerHandle,
			this,
			&AERCharacterBase::InterpMaxWalkSpeed,
			0.01f,
			true
		);
	}

}

void AERCharacterBase::StopEmotionMontage()
{
	// ���⼭�� ���� ���̱� ���� ����!
	// bIsPlayingEmotion, PlayingEmotionMontage�� EndDelegate���� ó��
	if (bIsPlayingEmotion && PlayingEmotionMontage)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		AnimInstance->Montage_Stop(0.2f, PlayingEmotionMontage);
	}
}

void AERCharacterBase::OnEmotionMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	// "��¥ ������ ����ǥ��"�� ������ ���� ���⸦ ���̰�!
	// ���� ����ǥ���� EndDelegate�� ����!
	if (Montage == PlayingEmotionMontage)
	{
		bIsPlayingEmotion = false;
		PlayingEmotionMontage = nullptr;
		if (Weapon)
		{
			Weapon->SetVisibility(true, true);
		}
	}
	
}

void AERCharacterBase::AttackHitCheck()
{
	TArray<FHitResult> OutHitResults;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	// ComboAttack ��
	float AttackRange = 260.0f;
	float AttackRadius = 100.0f;
	float AttackDamage = 300.0f;

	// �ٽ�: ���� ��Ÿ�ֿ� �´� ���� ���� ����
	if (CurrentComboActionData && CurrentComboActionData->MontageAttackInfoMap.Contains(CurrentComboMontage))
	{
		const FAttackInfo& Info = CurrentComboActionData->MontageAttackInfoMap[CurrentComboMontage];
		AttackRange = Info.Range;
		AttackRadius = Info.Radius;
		AttackDamage = Info.Damage;
	}

	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepMultiByChannel(
		OutHitResults,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_ERACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	if (HitDetected)
	{
		for (const FHitResult& Hit : OutHitResults)
		{
			FDamageEvent DamageEvent;
			Hit.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
		}
;	}

#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	//DrawDebugCapsule(
	//	GetWorld(),
	//	CapsuleOrigin,
	//	CapsuleHalfHeight,
	//	AttackRadius,
	//	FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
	//	DrawColor,
	//	false,
	//	5.0f
	//);
#endif
}

float AERCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	M_Stat->M_ApplyDamage(DamageAmount);

	return DamageAmount;
}

//void AERCharacterBase::M_SetupCharacterWidget(UM_UserWidget* M_InUserWidget)
//{
//	UMagnusHpBarWidget* M_HpBarWidget = Cast<UMagnusHpBarWidget>(M_InUserWidget);
//	if (M_HpBarWidget)
//	{
//		M_HpBarWidget->M_SetMaxHp(M_Stat->GetMaxHp());
//		M_HpBarWidget->M_UpdateHpBar(M_Stat->GetCurrentHp());
//		M_Stat->M_OnHpChanged.AddUObject(M_HpBarWidget, &UMagnusHpBarWidget::M_UpdateHpBar);
//
//	}
//}

void AERCharacterBase::M_SetupCharacterWidget(UM_UserWidget* M_InUserWidget)
{
	M_HpBarWidget = Cast<UMagnusHpBarWidget>(M_InUserWidget); // ��� ������ ���� ����
	if (M_HpBarWidget)
	{
		M_HpBarWidget->M_SetMaxHp(M_Stat->GetMaxHp());
		M_HpBarWidget->M_UpdateHpBar(M_Stat->GetCurrentHp());
		M_Stat->M_OnHpChanged.AddUObject(M_HpBarWidget, &UMagnusHpBarWidget::M_UpdateHpBar);
	}
}

void AERCharacterBase::SetDead()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	//�ٸ� Ű �Է� ����
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		DisableInput(PC);
	}

	if (M_DeadWidget)
	{
		UUserWidget* DeadWidgetInstance = CreateWidget<UUserWidget>(GetWorld(), M_DeadWidget);
		if (DeadWidgetInstance)
		{
			DeadWidgetInstance->AddToViewport();
		}
	}

}

void AERCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	AnimInstance->Montage_Play(DeadMontage, 1.0f);
}
