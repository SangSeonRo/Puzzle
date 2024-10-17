// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

#include "GMB_Puzzle.h"
#include "PC_Puzzle.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileGrid::ATileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATileGrid::BeginPlay()
{
	Super::BeginPlay();
}

void ATileGrid::MakeGrid()
{
	if(Tiles.Num() > 0)
	{
		for(int i = 0; i < Tiles.Num(); i++)
			Tiles[i]->Destroy();
		
		Tiles.Empty();	
	}
	Tiles.SetNum(GridRow * GridColumn);
	
	for(int i = 0; i < GridRow; i++)
	{
		for(int j = 0; j < GridColumn; j++)
		{
			int8 typeIndex = FMath::RandRange(0, Materials.Num()-1);
			ATile* tile = GetWorld()->SpawnActor<ATile>(TileClass, FVector(0, j*100, i*100), FRotator::ZeroRotator);
			if(tile)
			{
				tile->SetTile(typeIndex, Materials[typeIndex]);
				Tiles[i*GridColumn + j] = tile;
			}
		}
	}

	do
	{
		while(HasEmpty())
		{
			MoveTiles();
			FillGrid();
		}

		MatchingTileDestroy();
	}while(HasEmpty());
}

void ATileGrid::FillGrid()
{
	int lastIndex = GridRow * GridColumn - 1;
	for(int i = GridRow * (GridColumn-1); i < GridRow * GridColumn; i++)
	{
		if(Tiles[i] == nullptr)
		{
			int8 typeIndex = FMath::RandRange(0, Materials.Num()-1);
			ATile* tile = GetWorld()->SpawnActor<ATile>(TileClass, FVector(0, GetGridColumnFromTileIndex(i)*100, GetGridRowIndexFromTileIndex(i)*100), FRotator::ZeroRotator);
			if(tile)
			{
				tile->SetTile(typeIndex, Materials[typeIndex]);
				Tiles[i] = tile;
			}
		}
	}
}

TArray<ATile*> ATileGrid::SearchMatchingTile(ATile* tile)
{
	//UE_LOG(LogTemp, Warning, TEXT("Searching maching tile : %s"), *tile->GetName());
	
	int tileIndex = GetTileIndex(tile);
	int8 gridRowIndex = GetGridRowIndexFromTileIndex(tileIndex);
	int8 gridColumnIndex = GetGridColumnFromTileIndex(tileIndex);

	TArray<ATile*> HorizontalMatchTiles;
	HorizontalMatchTiles.Empty();
	HorizontalMatchTiles.Add(tile);

	int tempColumnIndex = gridColumnIndex;
	while(true)
	{
		--tempColumnIndex;
		if(tempColumnIndex<0)
			break;

		int targetIndex = GetTileIndexFromGridIndex(gridRowIndex,tempColumnIndex);

		if(Tiles[targetIndex] == nullptr)
			break;
		
		if(tile->IsMatching(Tiles[targetIndex]))
			HorizontalMatchTiles.Add(Tiles[targetIndex]);
		else
			break;
	}

	tempColumnIndex = gridColumnIndex;
	while(true)
	{
		++tempColumnIndex;
		if(tempColumnIndex >= GridColumn)
			break;

		int targetIndex = GetTileIndexFromGridIndex(gridRowIndex,tempColumnIndex);

		if(Tiles[targetIndex] == nullptr)
			break;
		
		if(tile->IsMatching(Tiles[targetIndex]))
			HorizontalMatchTiles.Add(Tiles[targetIndex]);
		else
			break;
	}
	
	TArray<ATile*> VerticalMatchTiles;
	VerticalMatchTiles.Empty();
	VerticalMatchTiles.Add(tile);

	int tempRowIndex = gridRowIndex;
	while(true)
	{
		--tempRowIndex;
		if(tempRowIndex<0)
			break;

		int targetIndex = GetTileIndexFromGridIndex(tempRowIndex,gridColumnIndex);

		if(Tiles[targetIndex] == nullptr)
			break;
		
		if(tile->IsMatching(Tiles[targetIndex]))
			VerticalMatchTiles.Add(Tiles[targetIndex]);
		else
			break;
	}

	tempRowIndex = gridRowIndex;
	while(true)
	{
		++tempRowIndex;
		if(tempRowIndex >= GridRow)
			break;
		
		int targetIndex = GetTileIndexFromGridIndex(tempRowIndex,gridColumnIndex);

		if(Tiles[targetIndex] == nullptr)
			break;
		
		if(tile->IsMatching(Tiles[targetIndex]))
			VerticalMatchTiles.Add(Tiles[targetIndex]);
		else
			break;
	}
	
	TArray<ATile*> returnValue;

	if(HorizontalMatchTiles.Num()>=3)
	{
		for(int i = 0; i < HorizontalMatchTiles.Num(); i++)
		{
			returnValue.Add(HorizontalMatchTiles[i]);
		}
	}
	if(VerticalMatchTiles.Num()>=3)
	{
		for(int i = 0; i < VerticalMatchTiles.Num(); i++)
		{
			if(returnValue.Find(VerticalMatchTiles[i]) == INDEX_NONE)
				returnValue.Add(VerticalMatchTiles[i]);
		}	
	}

	return returnValue;
}

int ATileGrid::MatchingTileDestroy()
{
	int DestoryTileCount = 0;
	TArray<ATile*> matchTiles;
	matchTiles.Empty();
	for(int i = 0; i < Tiles.Num(); i++)
	{
		if(Tiles[i] != nullptr)
		{
			TArray<ATile*> tempTiles = SearchMatchingTile(Tiles[i]);

			for(int j = 0; j < tempTiles.Num(); j++)
			{
				if(matchTiles.Find(tempTiles[j]) == INDEX_NONE)
					matchTiles.Add(tempTiles[j]);
			}
		}
	}
	DestoryTileCount = matchTiles.Num();	
	for(int i = 0; i < matchTiles.Num(); i++)
	{
		int targetTileIndex = GetTileIndex(matchTiles[i]);
		Tiles[targetTileIndex] = nullptr;
		matchTiles[i]->Destroy();
	}
	return DestoryTileCount;
}

bool ATileGrid::IsSwapAble(ATile* tile1, ATile* tile2)
{
	int tile1Index = GetTileIndex(tile1);
	int tile2Index = GetTileIndex(tile2);
	
	if(FMath::Abs(tile1Index - tile2Index) == 1 || FMath::Abs(tile1Index - tile2Index) == GridColumn)
		return true;

	return false;
}

void ATileGrid::UndoSwapTile(ATile* tile1, ATile* tile2)
{
	if(IsSwapAble(tile1, tile2))
	{
		FVector loc1 = tile1->GetActorLocation();
		tile1->SetActorLocation(tile2->GetActorLocation());
		tile2->SetActorLocation(loc1);

		int tile1Index = GetTileIndex(tile1);
		int tile2Index = GetTileIndex(tile2);
		
		Tiles[tile1Index] = tile2;
		Tiles[tile2Index] = tile1;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile"));
	}	
}

void ATileGrid::SwapTile(ATile* tile1, ATile* tile2)
{
	if(IsSwapAble(tile1, tile2))
	{
		FVector loc1 = tile1->GetActorLocation();
		tile1->SetActorLocation(tile2->GetActorLocation());
		tile2->SetActorLocation(loc1);

		int tile1Index = GetTileIndex(tile1);
		int tile2Index = GetTileIndex(tile2);
		
		Tiles[tile1Index] = tile2;
		Tiles[tile2Index] = tile1;

		int destroyTileCount = 0;
		do
		{
			while(HasEmpty())
			{
				MoveTiles();
				FillGrid();
			}

			destroyTileCount += MatchingTileDestroy();
		}while(HasEmpty());

		if(destroyTileCount == 0)
		{
			auto temppc = UGameplayStatics::GetPlayerController(GetWorld(),0);
			APC_Puzzle* pc = Cast<APC_Puzzle>(temppc);
			if(pc)
			{
				pc->UndoLastCommand();
			}
		}
		else
		{
			// auto tempMode = UGameplayStatics::GetGameMode(GetWorld());
			// AGMB_Puzzle* mode = Cast<AGMB_Puzzle>(tempMode);
			// if(mode)
			// {
			// 	mode->ObserverGameState->IncreasePlayerScore(destroyTileCount*10);
			// 	mode->ObserverGameState->DecreaseRemainCount();
			// }
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile"));
	}	
}

void ATileGrid::MoveTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveTiles"));

	for(int i = 1; i < GridRow; i++)
	{
		for(int j = 0; j < GridColumn; j++)
		{
			int tileIndex = GetTileIndexFromGridIndex(i,j);
			int targetIndex = GetTileIndexFromGridIndex(i-1,j);
			if(Tiles[tileIndex] != nullptr && Tiles[targetIndex] == nullptr)
			{
				Tiles[targetIndex] = Tiles[tileIndex];
				Tiles[targetIndex]->SetActorLocation(FVector(0, j*100, (i-1)*100));				
				Tiles[tileIndex] = nullptr;
			}
		}
	}
}

int ATileGrid::GetTileIndexFromGridIndex(int8 rowIndex, int8 columnIndex)
{
	return rowIndex * GridColumn + columnIndex;
}

int8 ATileGrid::GetGridRowIndexFromTileIndex(int tileIndex)
{
	return tileIndex / GridColumn;
}
int8 ATileGrid::GetGridColumnFromTileIndex(int tileIndex)
{
	return tileIndex % GridColumn;
}

int ATileGrid::GetTileIndex(ATile* tile)
{
	return Tiles.Find(tile);
}

bool ATileGrid::HasEmpty()
{
	for(int i = 0; i < Tiles.Num(); i++)
	{
		if(Tiles[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("HasEmpty"));
			return true;
		}
	}
	return false;
}
