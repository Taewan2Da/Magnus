// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/EREnemyCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Physics/ERCollision.h"
#include "ERComboActionData.h"
#include "CharacterStat/ERCharacterStatComponent.h"
#include "UI/ERWidgetComponent.h"
#include "UI/ERHpBarWidget.h"
#include "AI/ERAIController.h"
#include "EREnemyComboActionData.h"
#include "Engine/DamageEvents.h"
#include <Kismet/GameplayStatics.h>
#include "Game/ERGameMode.h"




// Sets default values
AEREnemyCharacterBase::AEREnemyCharacterBase()
{
	//// Pawn�� ȸ���� �����ϱ� ���� �⺻���� ����
	//bUseControllerRotationPitch = false;
	//bUseControllerRotationYaw = false;
	//bUseControllerRotationRoll = false;

	AIControllerClass = AERAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_ABCAPSULE);

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 900.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 200.0f;
	GetCharacterMovement()->GravityScale = 2.0f;

	//Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterMeshRef(
		TEXT("/Game/LiDailin/LiDailin.LiDailin"));
	if (CharacterMeshRef.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterMeshRef.Object);
	}

	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceClassRef(
		TEXT("/Game/LiDailin/Blueprints/ABP_ERLiDailin.ABP_ERLiDailin_C"));
	if (AnimInstanceClassRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceClassRef.Class);
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(
		TEXT("/Game/LiDailin/Animation/AM_Dead.AM_Dead"));
	if (DeadMontageRef.Object)
	{
		DeadMontage = DeadMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(
		TEXT("/Game/LiDailin/Animation/AM_OneTwo.AM_OneTwo"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<UERCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	HpBar = CreateDefaultSubobject<UERWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 220.0f));

	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(
		TEXT("/Game/Magnus/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::World); // screen
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//�� ī�޶� ����
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

}

void AEREnemyCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &AEREnemyCharacterBase::SetDead);
}

// Called when the game starts or when spawned
void AEREnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}


float AEREnemyCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	Stat->ApplyDamage(DamageAmount);

	return DamageAmount;
}

void AEREnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (HpBar)
	{
		// �÷��̾� ī�޶� �Ŵ��� ��������
		APlayerCameraManager* CameraManager = UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0);
		if (CameraManager)
		{
			// ī�޶��� ȸ���� ��������
			FRotator CameraRot = CameraManager->GetCameraRotation();

			// HpBar�� ���� ȸ���� ��������
			FRotator HpBarRot = HpBar->GetComponentRotation();

			// HpBar�� Pitch�� Roll�� ����, Yaw�� ī�޶�� ��ġ��Ű��
			FRotator NewRot = FRotator(HpBarRot.Pitch, CameraRot.Yaw + 180.0f, HpBarRot.Roll);

			// HpBar�� ȸ���� ����
			HpBar->SetWorldRotation(NewRot);
		}
	}
}

void AEREnemyCharacterBase::SetDead()
{
	IsAttacking = false;   // ���� ���� ���� ��� false��
	bIsCombo = false;
	CurrentCombo = 0;
	// ... ��Ÿ ���� ���� ������ ��� �ʱ�ȭ

	bIsDead = true;        // ���� ���� ���� true��

	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeadAnimation();
	SetActorEnableCollision(false);

	AERGameMode* GameMode = Cast<AERGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GameMode)
	{
		GameMode->DeadEnemyCount++;
		if (GameMode->DeadEnemyCount >= 30)
		{
			GameMode->ShowClearWidget();
		}
	}


	FTimerHandle DeadTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeadTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeadEventDelayTime, false);
}

void AEREnemyCharacterBase::PlayDeadAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->StopAllMontages(0.0f);
	UE_LOG(LogTemp, Warning, TEXT("StopAllMontages called"));
	float Result = AnimInstance->Montage_Play(DeadMontage, 2.0f);
	UE_LOG(LogTemp, Warning, TEXT("Montage_Play called, result: %f"), Result);
}

void AEREnemyCharacterBase::SetupCharacterWidget(UERUserWidget* InUserWidget)
{
	UERHpBarWidget* HpBarWidget = Cast<UERHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->SetMaxHp(Stat->GetMaxHp());
		HpBarWidget->UpdateHpBar(Stat->GetCurrentHp());
		Stat->OnHpChanged.AddUObject(HpBarWidget, &UERHpBarWidget::UpdateHpBar);
	}
}

float AEREnemyCharacterBase::GetAIPatrolRadius()
{
	return 800.0f;
}

float AEREnemyCharacterBase::GetAIDetectRange()
{
	return 400.0f;
}

float AEREnemyCharacterBase::GetAIAttackRange()
{
	//�ϴ� �ϵ��ڵ�
	return 200.0f;
}

float AEREnemyCharacterBase::GetAITurnSpeed()
{
	return 0.0f;
}

void AEREnemyCharacterBase::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void AEREnemyCharacterBase::AttackByAI()
{
	if (bIsDead) return; // ���� ���¸� ���� �Ұ�

	IsAttacking = true;

	ProcessComboCommand();
}

void AEREnemyCharacterBase::ProcessComboCommand()
{
	//if (CurrentCombo == 0)
	//{
	//	ComboActionBegin();
	//	return;
	//}

	//if (!ComboTimerHandle.IsValid())
	//{
	//	HasNextComboCommand = false;
	//}

	//else
	//{
	//	HasNextComboCommand = true;
	//}

	++CurrentCombo;

	if (EnemyComboActionData->MaxComboCount < CurrentCombo)
	{
		CurrentCombo = 1;
	}

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	//CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, EnemyComboActionData->MaxComboCount);
	FName SectionToPlay = *FString::Printf(TEXT("%s%d"), *EnemyComboActionData->MontageSectionNamePrefix, CurrentCombo);
	AnimInstance->Montage_JumpToSection(SectionToPlay, ComboActionMontage);
	//SetComboCheckTimer();
	//HasNextComboCommand = false;

	GetWorld()->GetTimerManager().ClearTimer(ComboEndTimerHandle);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AEREnemyCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);


	//ComboEndTimerHandle.Invalidate();

	//ComboTimerHandle.Invalidate();
	//SetComboCheckTimer();
}

void AEREnemyCharacterBase::ComboActionBegin()
{
	// Combo Status
	CurrentCombo = 1;

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	// Animation Setting
	const float AttackSpeedRate = 1.0f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &AEREnemyCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();

}

void AEREnemyCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{

	NotifyComboActionEnd();

	//ensure(CurrentCombo != 0);
	//CurrentCombo = 0;
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	GetWorld()->GetTimerManager().SetTimer(
		 ComboEndTimerHandle,
		[&]()
		{
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			CurrentCombo = 0;
		}

	, 0.5f, false);

}

void AEREnemyCharacterBase::NotifyComboActionEnd()
{
	OnAttackFinished.ExecuteIfBound();
}


void AEREnemyCharacterBase::SetComboCheckTimer()
{
	//GEngine->AddOnScreenDebugMessage(1, 1.f, FColor::Blue, TEXT("ComboCheck"));
	// SetComboCheckTimer() �Լ� ����
	if (EnemyComboActionData) // null üũ
	{
		int32 ComboIndex = CurrentCombo - 1;
		ensure(EnemyComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));

		const float AttackSpeedRate = 1.0f;
		float ComboEffectiveTime = (EnemyComboActionData->EffectiveFrameCount[ComboIndex] / EnemyComboActionData->FrameRate) / AttackSpeedRate;
		if (ComboEffectiveTime > 0.0f)
		{
			GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &AEREnemyCharacterBase::ComboCheck, ComboEffectiveTime, false);
		}
	}

}

void AEREnemyCharacterBase::ComboCheck()
{
	//GEngine->AddOnScreenDebugMessage(0, 1.f, FColor::Red, TEXT("ComboCheck"));
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1, EnemyComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *EnemyComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		SetComboCheckTimer();
		HasNextComboCommand = false;

	}
}

void AEREnemyCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), true, this);

	const float AttackRange = 50.0f;
	const float AttackRadius = 50.0f;
	const float AttackDamage = 100.0f;
	const FVector Start = GetActorLocation() + GetActorForwardVector() * GetCapsuleComponent()->GetScaledCapsuleRadius();
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(
		OutHitResult,
		Start,
		End,
		FQuat::Identity,
		CCHANNEL_ERACTION,
		FCollisionShape::MakeSphere(AttackRadius),
		Params
	);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}
#if ENABLE_DRAW_DEBUG
	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = AttackRange * 0.5f;
	FColor DrawColor = HitDetected ? FColor::Green : FColor::Red;

	/*DrawDebugCapsule(
		GetWorld(),
		CapsuleOrigin,
		CapsuleHalfHeight,
		AttackRadius,
		FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(),
		DrawColor,
		false,
		5.0f
	);*/
#endif
}





