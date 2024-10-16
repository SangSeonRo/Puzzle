// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Puzzle.h"

#include "GI_Puzzle.h"
#include "Tile.h"
#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

void APC_Puzzle::SetupInputComponent()
{
	Super::SetupInputComponent();

	// Enhanced Input Subsystem 가져오기
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// 매핑 컨텍스트를 추가하여 입력 활성화
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Move 액션 바인딩 (축 입력)
		EnhancedInputComponent->BindAction(IA_Select, ETriggerEvent::Completed, this, &APC_Puzzle::InputSelect);
	}
}

void APC_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void APC_Puzzle::InputSelect(const FInputActionValue& Value)
{
	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
		auto gameInstance = Cast<UGI_Puzzle>(GetWorld()->GetGameInstance());
		if (gameInstance)
		{
			if (HitResult.bBlockingHit)
			{
				ATile* tile = Cast<ATile>(HitResult.GetActor());
				gameInstance->SetSelectedTile(tile);
			}		
			else
			{			
				gameInstance->SetSelectedTile(nullptr);
			}
		}
	}
}
