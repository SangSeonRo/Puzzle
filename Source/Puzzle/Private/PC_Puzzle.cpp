// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Puzzle.h"

#include "Tile.h"
#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

void APC_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	SelectedTile0 = nullptr;
	SelectedTile1 = nullptr;
	
	//마우스포인터 활성화.
	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);	
}

bool APC_Puzzle::SetTileGrid()
{
	auto findTileGrid =  UGameplayStatics::GetActorOfClass(GetWorld(), ATileGrid::StaticClass());
	if(findTileGrid)
	{
		TileGrid = Cast<ATileGrid>(findTileGrid);
		if(TileGrid)
			return true;
	}
	return false;
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
		if(TileGrid == nullptr)
			SetTileGrid();
			
		ATile* selectedTile = Cast<ATile>(HitResult.GetActor());
		
		if(selectedTile)
		{
			UE_LOG(LogTemp, Display, TEXT("SelectedTile : %s, GridIndex : %d"), *selectedTile->GetName(), TileGrid->GetTileIndex(selectedTile));
			
			if(!SelectedTile0.IsValid())
			{
				SelectedTile0 = selectedTile;
				SelectedTile0->Selected(true);
				UE_LOG(LogTemp, Display, TEXT("SelectedTile0 : %s, GridIndex : %d"), *SelectedTile0->GetName(), TileGrid->GetTileIndex(SelectedTile0.Get()));
			}
			else if(!SelectedTile1.IsValid())
			{
				if(TileGrid->IsAdjustTiles(SelectedTile0.Get(), selectedTile))
				{
					SelectedTile1 = selectedTile;
					SelectedTile1->Selected(true);
					UE_LOG(LogTemp, Display, TEXT("SelectedTile1 : %s, GridIndex : %d"), *SelectedTile1->GetName(), TileGrid->GetTileIndex(SelectedTile1.Get()));

					TileGrid->SwapProcess(SelectedTile0.Get(), SelectedTile1.Get());

					SelectedTile0->Selected(false);
					SelectedTile1->Selected(false);
					SelectedTile0 = nullptr;
					SelectedTile1 = nullptr;
				}
				else
				{
					SelectedTile0->Selected(false);
					SelectedTile0 = nullptr;
				}				
			}
		}
	}
}
