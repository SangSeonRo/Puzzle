// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Puzzle.h"
#include "SwapTilesCommand.h"
#include "Tile.h"
#include "EnhancedInputComponent.h"

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
		Select(HitResult.GetActor());
	}
}

void APC_Puzzle::Select(AActor* selectedActor)
{
	if(selectedActor != nullptr && selectedActor->IsA(ATile::StaticClass()))
	{
		ATile* SelectedTile = Cast<ATile>(selectedActor);

		if(SelectTiles.Find(SelectedTile) == INDEX_NONE)
		{
			SelectedTile->SelectTile(true);
			SelectTiles.Add(SelectedTile);
		}
		
		if(SelectTiles.Num() > 2)
		{
			SelectTiles[0]->SelectTile(false);
			SelectTiles.RemoveAt(0);
		}

		if(SelectTiles.Num() == 2)
		{	
			ExcuteCommand();

			while(SelectTiles.Num() > 0)
			{
				SelectTiles[0]->SelectTile(false);
				SelectTiles.RemoveAt(0);
			}
		}
	}
	else
	{
		while(SelectTiles.Num() > 0)
		{
			SelectTiles[0]->SelectTile(false);
			SelectTiles.RemoveAt(0);
		}
	}		
}

void APC_Puzzle::ExcuteCommand()
{
	ICommand* SwapCommand = new SwapTilesCommand(SelectTiles[0],SelectTiles[1]);
	SwapCommand->Execute();
	CommandHistory.Push(SwapCommand);
}

void APC_Puzzle::UndoLastCommand()
{
	if(CommandHistory.Num() > 0)
	{
		CommandHistory.Last()->Undo();
		CommandHistory.Pop();		
	}
}