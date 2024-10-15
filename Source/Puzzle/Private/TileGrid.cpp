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

// Called every frame
void ATileGrid::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

