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

	PreSelectedTile = SelectedTile = nullptr;
}

void UGI_Puzzle::SetSelectedTile(ATile* Tile)
{
	if (Tile != nullptr)
	{
		if(PreSelectedTile != nullptr)
			PreSelectedTile->SelectTile(false);

		if(SelectedTile == Tile)
		{
			SelectedTile->SelectTile(false);
			PreSelectedTile = SelectedTile = nullptr;
			return;
		}
		
		PreSelectedTile = SelectedTile;
		if(PreSelectedTile != nullptr)
			PreSelectedTile->SelectTile(true);
		
		SelectedTile = Tile;
		SelectedTile->SelectTile(true);
		
		if(SelectedTile && PreSelectedTile)
		{
			auto temp = UGameplayStatics::GetActorOfClass(GetWorld(), ATileGrid::StaticClass());

			ATileGrid* tileGrid = Cast<ATileGrid>(temp);

			if(tileGrid)
			{
				tileGrid->SwapTile(SelectedTile,PreSelectedTile);
			}

			PreSelectedTile->SelectTile(false);
			SelectedTile->SelectTile(false);
			PreSelectedTile = SelectedTile = nullptr;
		}
	}
	else
	{
		
	}	
}
