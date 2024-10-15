// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Puzzle.h"

#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

UGI_Puzzle::UGI_Puzzle()
{
	PlayerScore = 0;
	RemainingMoves = 30;
}

void UGI_Puzzle::AddScore(int32 points)
{
	PlayerScore+=points;
}

void UGI_Puzzle::DecreaseMoves()
{
	if(RemainingMoves>0)
	{
		RemainingMoves--;
	}	
}

void UGI_Puzzle::ResetGameState()
{
	PlayerScore = 0;
	RemainingMoves = 30;
}

void UGI_Puzzle::SetSelectedTile(ATile* Tile)
{
	if (Tile != nullptr)
	{
		PreSelectedTile = SelectedTile;
		SelectedTile = Tile;

		if(SelectedTile && PreSelectedTile)
		{
			auto temp = UGameplayStatics::GetActorOfClass(GetWorld(), ATileGrid::StaticClass());

			ATileGrid* tileGrid = Cast<ATileGrid>(temp);

			if(tileGrid)
			{
				tileGrid->SwapTile(SelectedTile,PreSelectedTile);
			}
			
			UE_LOG(LogTemp, Display, TEXT("Select Tile : %s"), *SelectedTile->GetName());
			UE_LOG(LogTemp, Display, TEXT("PreSelect Tile : %s"), *PreSelectedTile->GetName());

			PreSelectedTile = SelectedTile = nullptr;
		}
	}
	else
	{
		
	}	
}
