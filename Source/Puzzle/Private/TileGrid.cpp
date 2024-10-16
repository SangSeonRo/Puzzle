// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

#include "Chaos/PBDRigidsEvolution.h"
#include "Debug/ReporterGraph.h"
#include "SceneQueries/SceneSnappingManager.h"

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

	MakeGrid();
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

TArray<ATile*> ATileGrid::SearchMachingTile(ATile* tile)
{
	//Grid[tile->RowIndex][tile->ColumnIndex]
	UE_LOG(LogTemp, Warning, TEXT("Searching maching tile : %s"), *tile->GetName());
	
	int tileIndex = Tiles.Find(tile);
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
	
	UE_LOG(LogTemp, Warning, TEXT("maching_Horizontal : %d"), HorizontalMatchTiles.Num());
	UE_LOG(LogTemp, Warning, TEXT("maching_Vertical : %d"), VerticalMatchTiles.Num());

	TArray<ATile*> returnValue;
	returnValue.Empty();

	if(HorizontalMatchTiles.Num() >= 3)
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

	HorizontalMatchTiles.Empty();
	VerticalMatchTiles.Empty();

	return returnValue;
}

void ATileGrid::MatchingTileDestroy()
{
	TArray<ATile*> matchTiles;
	matchTiles.Empty();
	for(int i = 0; i < Tiles.Num(); i++)
	{
		if(Tiles[i] != nullptr)
		{
			TArray<ATile*> tempTiles = SearchMachingTile(Tiles[i]);

			for(int j = 0; j < tempTiles.Num(); j++)
			{
				if(matchTiles.Find(tempTiles[j]) == INDEX_NONE)
					matchTiles.Add(tempTiles[j]);
			}
		}
	}
		
	for(int i = 0; i < matchTiles.Num(); i++)
	{
		int targetTileIndex = Tiles.Find(matchTiles[i]);
		Tiles[targetTileIndex] = nullptr;
		matchTiles[i]->Destroy();
	}
}

void ATileGrid::SwapTile(ATile* tile1, ATile* tile2)
{
	int tile1Index = Tiles.Find(tile1);
	int tile2Index = Tiles.Find(tile2);

	if(FMath::Abs(tile1Index - tile2Index) == 1 || FMath::Abs(tile1Index - tile2Index) == GridColumn)
	{
		FVector loc1 = tile1->GetActorLocation();
		tile1->SetActorLocation(tile2->GetActorLocation());
		tile2->SetActorLocation(loc1);

		Tiles[tile1Index] = tile2;
		Tiles[tile2Index] = tile1;

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
