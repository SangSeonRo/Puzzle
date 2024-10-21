// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Puzzle.h"

void APC_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	//마우스포인터 활성화.
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void APC_Puzzle::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Subsystem 가져오기
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// 매핑 컨텍스트를 추가하여 입력 활성화
		Subsystem->ClearAllMappings();
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}

	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Move 액션 바인딩 (축 입력)
		EnhancedInputComponent->BindAction(IA_Mouse_BT_Left, ETriggerEvent::Completed, this, &APC_Puzzle::InputAction_Mouse_BT_Left);
	}
}

void APC_Puzzle::InputAction_Mouse_BT_Left(const FInputActionValue& value)
{
	FHitResult HitResult;
	GetHitResultUnderCursor(ECC_Visibility, false, HitResult);

	if (HitResult.bBlockingHit)
	{
		//타일그리드로 전달.
		//HitResult.GetActor()
	}
}
