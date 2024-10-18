// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

#include "Tile.h"

// Sets default values
ATileGrid::ATileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

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

void ATileGrid::MakeGrid()
{
	//기존에 그려진 그리드가 있다면 삭제.
	if( TileGrid.Num() > 0 )
	{
		for( int i = 0; i < TileGrid.Num(); i++ )
		{
			if( TileGrid[i].Num() > 0 )
			{
				for(int j = 0; j < TileGrid[i].Num(); j++)
				{
					TileGrid[i][j]->Destroy();
				}

				TileGrid[i].Empty();
			}
		}
		TileGrid.Empty();
	}

	//새로운 그리드 생성.
	TileGrid.SetNum(GridRow);
	for(int i = 0; i < TileGrid.Num(); i++)
	{
		TileGrid[i].SetNum(GridColumn);
		for(int j = 0; j < TileGrid[i].Num(); j++)
		{
			int typeIndex = FMath::RandRange(0, Materials.Num() - 1);
			ATile* tile = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), FVector(0, j*100, i*100), FRotator::ZeroRotator);
			tile->SetTile(typeIndex, Materials[typeIndex]);
			TileGrid[i][j] = tile;
		}
	}
}