// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

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

	if(Grid.Num()>0)
	{
		for(int i = 0; i < Grid.Num(); i++)
		{
			Grid[i].Empty();
		}
		Grid.Empty();
	}

	Grid.SetNum(GridRow);
	for(int i = 0; i < GridRow; i++)
		Grid[i].SetNum(GridColumn);
	
	for(int i = 0; i < GridRow; i++)
	{
		for(int j = 0; j < GridColumn; j++)
		{
			int8 typeIndex = FMath::RandRange(0, Materials.Num()-1);
			ATile* tile = GetWorld()->SpawnActor<ATile>(TileClass, FVector(0, j*100, i*100), FRotator::ZeroRotator);
			if(tile)
			{
				tile->SetTile(typeIndex, i, j, Materials[typeIndex]);
				Tiles.Add(tile);
				Grid[i][j] = tile;
			}
		}
	}		
}

void ATileGrid::SearchMachingTile(ATile* tile)
{
	//Grid[tile->RowIndex][tile->ColumnIndex]
	UE_LOG(LogTemp, Warning, TEXT("Searching maching tile : %s"), *tile->GetName());

	TArray<ATile*> matRowTiles;
	matRowTiles.Empty();
	matRowTiles.Add(tile);
	int8 i = tile->RowIndex;
	while(true)
	{
		--i;
		if(i<0)
			break;

		if(Grid[i][tile->ColumnIndex] == nullptr)
			break;
		if(tile->TypeIndex == Grid[i][tile->ColumnIndex]->TypeIndex)
			matRowTiles.Add(Grid[i][tile->ColumnIndex]);
		else
			break;
	}
	
	i = tile->RowIndex;
	while(true)
	{
		++i;
		if(i > Grid.Num()-1)
			break;

		if(Grid[i][tile->ColumnIndex] == nullptr)
			break;
		if(tile->TypeIndex == Grid[i][tile->ColumnIndex]->TypeIndex)
			matRowTiles.Add(Grid[i][tile->ColumnIndex]);
		else
			break;
	}
	
	TArray<ATile*> matColumnTiles;
	matColumnTiles.Empty();
	matColumnTiles.Add(tile);
	i = tile->ColumnIndex;
	while(true)
	{
		--i;
		if(i<0)
			break;

		if(Grid[tile->RowIndex][i] == nullptr)
			break;
			
		if(tile->TypeIndex == Grid[tile->RowIndex][i]->TypeIndex)
			matColumnTiles.Add(Grid[tile->RowIndex][i]);
		else
			break;
	}
	
	i = tile->ColumnIndex;
	while(true)
	{
		++i;
		if(i > Grid[tile->RowIndex].Num()-1)
			break;

		if(Grid[tile->RowIndex][i] == nullptr)
			break;
		
		if(tile->TypeIndex == Grid[tile->RowIndex][i]->TypeIndex)
			matColumnTiles.Add(Grid[tile->RowIndex][i]);
		else
			break;
	}

	UE_LOG(LogTemp, Warning, TEXT("maching Row tile : %d"), matRowTiles.Num());
	UE_LOG(LogTemp, Warning, TEXT("maching Column tile : %d"), matColumnTiles.Num());
	
	if(matRowTiles.Num() >= 3)
	{
		for(int j = 0; j < matRowTiles.Num(); j++)
		{
			Tiles.Remove(matRowTiles[j]);
			Grid[matRowTiles[j]->RowIndex][matRowTiles[j]->ColumnIndex] = nullptr;
			matRowTiles[j]->Destroy();
		}		
	}
	matRowTiles.Empty();

	if(matColumnTiles.Num()>=3)
	{
		for(int j = 0; j < matColumnTiles.Num(); j++)
		{
			Tiles.Remove(matColumnTiles[j]);
			Grid[matColumnTiles[j]->RowIndex][matColumnTiles[j]->ColumnIndex] = nullptr;
			matColumnTiles[j]->Destroy();
		}	
	}
	matColumnTiles.Empty();
}

void ATileGrid::SwapTile(ATile* tile1, ATile* tile2)
{
	if((tile1->RowIndex == tile2->RowIndex && FMath::Abs(tile1->ColumnIndex - tile2->ColumnIndex) == 1)
	||(tile1->ColumnIndex == tile2->ColumnIndex && FMath::Abs(tile1->RowIndex - tile2->RowIndex) == 1))
	{
		FVector loc1 = tile1->GetActorLocation();
		tile1->SetActorLocation(tile2->GetActorLocation());
		tile2->SetActorLocation(loc1);
	
		int8 tempRowIndex = tile1->RowIndex;
		tile1->RowIndex = tile2->RowIndex;
		tile2->RowIndex = tempRowIndex;

		int8 tempColumnIndex = tile1->ColumnIndex;
		tile1->ColumnIndex = tile2->ColumnIndex;
		tile2->ColumnIndex = tempColumnIndex;;

		Grid[tile1->RowIndex][tile1->ColumnIndex] = tile1;
		Grid[tile2->RowIndex][tile2->ColumnIndex] = tile2;

		SearchMachingTile(tile1);
		SearchMachingTile(tile2);

		MoveTiles();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile"));
	}	
}
void ATileGrid::MoveTiles()
{
	UE_LOG(LogTemp, Warning, TEXT("MoveTiles"));

	for(int i = 1; i < Grid.Num(); i++)
	{
		for(int j = 0; j < Grid[i].Num(); j++)
		{
			if(Grid[i-1][j] == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("Grid[%d][%d]==nullptr"), i-1, j);
				Grid[i-1][j] = Grid[i][j];
				Grid[i-1][j]->RowIndex = (i-1);
				Grid[i-1][j]->ColumnIndex = j;
				Grid[i-1][j]->SetActorLocation(FVector(0, j*100, (i-1)*100));				
				Grid[i][j] = nullptr;	
			}
		}
	}
}