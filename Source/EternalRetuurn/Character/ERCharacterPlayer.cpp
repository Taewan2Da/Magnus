// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/ERCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/CharacterMovementComponent.h"


AERCharacterPlayer::AERCharacterPlayer()
{
	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;


	//Input 
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> InputMappingContextRef(TEXT
	("/Script/EnhancedInput.InputMappingContext'/Game/ThirdPerson/Input/IMC_Default.IMC_Default'"));
	if (InputMappingContextRef.Object)
	{
		DefaultMappingContext = InputMappingContextRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionMoveRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Move.IA_Move'"));
	if (InputActionMoveRef.Object)
	{
		MoveAction = InputActionMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Jump.IA_Jump'"));
	if (InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLookRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Look.IA_Look'"));
	if (InputActionLookRef.Object)
	{
		LookAction = InputActionLookRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Attack.IA_Attack'"));
	if (InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputBasicActionAttackRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_BasicAttack.IA_BasicAttack'"));
	if (InputBasicActionAttackRef.Object)
	{
		BasicAttackAction = InputBasicActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputEmotion1Ref(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Emotion1.IA_Emotion1'"));
	if (InputEmotion1Ref.Object)
	{
		EmotionAction1 = InputEmotion1Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputEmotion2Ref(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Emotion2.IA_Emotion2'"));
	if (InputEmotion2Ref.Object)
	{
		EmotionAction2 = InputEmotion2Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputSkill1Ref(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Skill1.IA_Skill1'"));
	if (InputSkill1Ref.Object)
	{
		SkillAction1 = InputSkill1Ref.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputRollRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Roll.IA_Roll'"));
	if (InputRollRef.Object)
	{
		RollAction = InputRollRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputSprintRef(TEXT
	("/Script/EnhancedInput.InputAction'/Game/ThirdPerson/Input/Actions/IA_Sprint.IA_Sprint'"));
	if (InputSprintRef.Object)
	{
		SprintAction = InputSprintRef.Object;
	}

}

void AERCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();

	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = 
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AERCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{

	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Move);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Look);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::MyJump);
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Attack);
	EnhancedInputComponent->BindAction(BasicAttackAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::BasicAttack);


	EnhancedInputComponent->BindAction(EmotionAction1, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Emotion1);
	EnhancedInputComponent->BindAction(EmotionAction2, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Emotion2);

	EnhancedInputComponent->BindAction(SkillAction1, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Skill1);
	EnhancedInputComponent->BindAction(RollAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Roll);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &AERCharacterPlayer::Sprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &AERCharacterPlayer::StopSprint);
}

void AERCharacterPlayer::Move(const FInputActionValue& Value)
{
	if (bIsPlayingEmotion)
	{
		StopEmotionMontage();
		// 감정표현 중단 후 이동 실행
	}

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AERCharacterPlayer::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

}

void AERCharacterPlayer::MyJump(const FInputActionValue& Value)
{
	if (bIsPlayingEmotion)
	{
		StopEmotionMontage();
		// 감정표현 중단 후 이동 실행
	}

	ACharacter::Jump();
}

void AERCharacterPlayer::Attack()
{
	if (bIsPlayingEmotion)
	{
		StopEmotionMontage();
		// 감정표현 중단 후 공격 실행
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bIsRolling) return;

	CurrentComboMontage = ComboActionMontage;
	CurrentComboActionData = ComboActionData;
	ProcessComboCommand();
}

void AERCharacterPlayer::BasicAttack()
{
	if (bIsPlayingEmotion)
	{
		StopEmotionMontage();
		// 감정표현 중단 후 점프 실행
	}

	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bIsRolling) return;

	CurrentComboMontage = ComboActionMontage2;
	CurrentComboActionData = ComboActionData2;
	ProcessComboCommand();
}

void AERCharacterPlayer::Emotion1()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bIsRolling) return;

	CurrentEmotion = EmotionAsset1;
	EmotionPlay();
}

void AERCharacterPlayer::Emotion2()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bIsRolling) return;

	CurrentEmotion = EmotionAsset2;
	EmotionPlay();
}

void AERCharacterPlayer::Skill1()
{
	if (GetCharacterMovement()->IsFalling())
	{
		return;
	}

	if (bIsPlayingEmotion)
	{
		StopEmotionMontage();
		// 감정표현 중단 후 점프 실행
	}

	if (bIsRolling) return;

	CurrentSkill = SkillAsset1;
	SkillPlay();
}

//void AERCharacterPlayer::Roll()
//{
//	RollPlay();
//}

void AERCharacterPlayer::Roll()
{
	if (GetCharacterMovement()->IsFalling() || bIsRolling)
		return;

	bIsRolling = true;
	RollPlay();

	// 롤 애니메이션이 끝나면 bIsRolling을 false로 만들어야 함
	float RollDuration = 0.735f;
	GetWorld()->GetTimerManager().SetTimer(
		RollTimerHandle, this, &AERCharacterPlayer::OnRollEnd, RollDuration, false);
}


void AERCharacterPlayer::OnRollEnd()
{
	bIsRolling = false;
}

void AERCharacterPlayer::Sprint()
{
	StartSprint();
}

