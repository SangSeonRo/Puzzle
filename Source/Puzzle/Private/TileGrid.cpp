// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

#include "Tile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ATileGrid::ATileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TileClass = ATile::StaticClass();
	SetMaterials();
}

void ATileGrid::SetMaterials()
{
	TArray<FString> MaterialPathStrings;
	MaterialPathStrings.Add(TEXT("/Script/Engine.Material'/Game/Material/Mat_Red.Mat_Red'"));
	MaterialPathStrings.Add(TEXT("/Script/Engine.Material'/Game/Material/Mat_Yellow.Mat_Yellow'"));
	MaterialPathStrings.Add(TEXT("/Script/Engine.Material'/Game/Material/Mat_Green.Mat_Green'"));
	MaterialPathStrings.Add(TEXT("/Script/Engine.Material'/Game/Material/Mat_Sky.Mat_Sky'"));
	MaterialPathStrings.Add(TEXT("/Script/Engine.Material'/Game/Material/Mat_Blue.Mat_Blue'"));
	MaterialPathStrings.Add(TEXT("/Script/Engine.Material'/Game/Material/Mat_Purple.Mat_Purple'"));

	for(FString MaterialPath : MaterialPathStrings)
	{
		ConstructorHelpers::FObjectFinder<UMaterialInterface> matFinder(*MaterialPath);
		if (matFinder.Succeeded())
			Materials.Add(matFinder.Object);
	}
}

// Called when the game starts or when spawned
void ATileGrid::BeginPlay()
{
	Super::BeginPlay();
}

void ATileGrid::DestroyAllTiles()
{
	//그리드에 쓰인 타일 삭제.
	if( TileGrid.Num() > 0 )
	{
		for( int i = 0; i < TileGrid.Num(); i++ )
		{
			if( TileGrid[i].Num() > 0 )
			{
				for(int j = 0; j < TileGrid[i].Num(); j++)
				{
					TileGrid[i][j]->Destroy();
					TileGrid[i][j].Reset();					
				}
				TileGrid[i].Empty();
			}
		}
		TileGrid.Empty();
	}

	//Unused타일 삭제.
	if(UnusedTiles.Num()>0)
	{
		for( int i = 0; i < UnusedTiles.Num(); i++ )
		{	
			UnusedTiles[i]->Destroy();
		}
		UnusedTiles.Empty();
	}		
}

void ATileGrid::MakeGrid()
{
	DestroyAllTiles();

	//새로운 그리드 생성.
	TileGrid.SetNum(GridRow);
	for(int i = 0; i < TileGrid.Num(); i++)
	{
		TileGrid[i].SetNum(GridColumn);
		for(int j = 0; j < TileGrid[i].Num(); j++)
		{
			int typeIndex = FMath::RandRange(0, Materials.Num() - 1);
			ATile* tile = GetWorld()->SpawnActor<ATile>(TileClass, FVector(0, j*TileWidth+(TileWidth/2.0f), i*TileHeight+(TileHeight/2.0f)), FRotator::ZeroRotator);
			if(tile)
			{
				tile->SetTile(typeIndex, Materials[typeIndex]);
				TileGrid[i][j] = tile;	
			}			
		}
	}
}

void ATileGrid::InitializeTileGrid(int32 gridRow, int32 gridColumn)
{
	GridRow = gridRow;
	GridColumn = gridColumn;
	
	DestroyAllTiles();
	MakeGrid();
}

FVector ATileGrid::GetCameraPosition()
{
	return FVector(GridColumn, GridRow, 0);
}