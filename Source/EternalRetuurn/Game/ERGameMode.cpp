// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/ERGameMode.h"
#include "Player/ERPlayerController.h"
#include "Kismet/GameplayStatics.h"

AERGameMode::AERGameMode()
{


	static ConstructorHelpers::FClassFinder<APawn> DefaultPawnClassRef(TEXT
	("/Script/Engine.Blueprint'/Game/Magnus/Blueprints/BP_ERCharacterPlayer.BP_ERCharacterPlayer_C'"));

	if (DefaultPawnClassRef.Class)
	{
		DefaultPawnClass = DefaultPawnClassRef.Class;
	}

	
	//DefaultPawnClass
	static ConstructorHelpers::FClassFinder<AERPlayerController> PlayerControllerClassRef(
		TEXT("/Script/EternalRetuurn.ERPlayerController"));

	if (PlayerControllerClassRef.Class)
	{
		PlayerControllerClass = PlayerControllerClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassRef(
		TEXT("/Game/Magnus/UI/WBP_GameClear.WBP_GameClear_C"));
	if (WidgetClassRef.Class)
	{
		ClearWidgetClass = WidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> S_WidgetClassRef(
	TEXT("/Game/Magnus/UI/WBP_Start.WBP_Start_C"));
	if (S_WidgetClassRef.Class)
	{
		StartWidgetClass = S_WidgetClassRef.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> D_WidgetClassRef(
	TEXT("/Game/Magnus/UI/WBP_Dead.WBP_Dead_C"));
	if (D_WidgetClassRef.Class)
	{
		DeadWidgetClass = D_WidgetClassRef.Class;
	}

}

void AERGameMode::ShowClearWidget()
{
	if (ClearWidgetClass)
	{
		UUserWidget* ClearWidget = CreateWidget(GetWorld(), ClearWidgetClass);
		if (ClearWidget)
		{
			ClearWidget->AddToViewport();

			// 게임 일시정지
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
			if (PC)
			{
				PC->bShowMouseCursor = true;
				PC->SetInputMode(FInputModeUIOnly());
			}
		
		}
	}
	

}
