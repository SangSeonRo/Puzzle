// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

#include "Tile.h"
#include "Components/SceneComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ATileGrid::ATileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TileClass = ATile::StaticClass();
	
    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
}

void ATileGrid::SetMaterials(const TArray<UMaterialInterface*>& materials)
{
	Materials = materials;
}

// Called when the game starts or when spawned
void ATileGrid::BeginPlay()
{
	Super::BeginPlay();
}

void ATileGrid::InitializeTileGrid(int32 gridRow, int32 gridColumn)
{
	GridRow = gridRow;
	GridColumn = gridColumn;
	
	TileGridDestroyAll();
	MakeTileGrid();
}

void ATileGrid::TileGridDestroyAll()
{
	for(TWeakObjectPtr<ATile> tile : TileGrid)
	{
		if(tile.IsValid())
			tile->Destroy();
	}
	TileGrid.Empty();

	for(ATile* tile : UnusedTiles)
	{
		if(tile)
			tile->Destroy();
	}
	UnusedTiles.Empty();
}

void ATileGrid::MakeTileGrid()
{
	if(Materials.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("ATileGrid::MakeTileGrid: Materials is empty"));
		return;
	}
	
	//원점을 기준으로 중앙 정렬.
	SetActorLocation(FVector(0,-(GridRow*TileWidth/2)+(TileWidth/2),-(GridColumn*TileHeight/2)+(TileHeight/2)));

	//새로운 그리드 생성.
	TileGrid.SetNum(GridRow*GridColumn);
	for(int index = 0; index < TileGrid.Num(); index++)
	{
		int typeIndex = FMath::RandRange(0, Materials.Num() - 1);
		ATile* tile = GetWorld()->SpawnActor<ATile>(TileClass);
		if(tile)
		{
			tile->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
			tile->SetActorRelativeLocation(FVector(0, GetGridColumnFromTileIndex(index)*TileWidth, GetGridRowIndexFromTileIndex(index)*TileHeight));
			tile->SetTile(typeIndex, Materials[typeIndex]);
			TileGrid[index] = tile;	
		}			
	}

	do
	{
		while(HasEmpty())
		{
			MoveTiles();
			FillGrid();
		}
		SearchMatchingTiles();
		ProcessMatchingTiles();
	}
	while (HasEmpty());

	if(IsMatchPossible() == false)
	{
		TileGridDestroyAll();
		MakeTileGrid();
	}		
}

bool ATileGrid::IsMatchPossible()
{
	// 매칭가능성 체크
	for(int index = 0; index < TileGrid.Num(); index++)
	{
		TMap<int32,int32> matchCount;
		matchCount.Empty();

		TArray<TPair<int32, int32>> adjustIndexes = {
			TPair<int32, int32>(index - 1, index - 2),
			TPair<int32, int32>(index + 1, index + 2),
			TPair<int32, int32>(index+GridColumn, index+GridColumn*2),
			TPair<int32, int32>(index-GridColumn, index-GridColumn*2)
		};
		
		for (const TPair<int32, int32>& Pair : adjustIndexes)
		{
			if(TileGrid.IsValidIndex(Pair.Key))
			{
				if(matchCount.Contains(TileGrid[Pair.Key]->TypeIndex))
					matchCount[TileGrid[Pair.Key]->TypeIndex]++;
				else
				{
					matchCount.Add(TileGrid[Pair.Key]->TypeIndex, 1);
				}

				if(TileGrid.IsValidIndex(Pair.Value))
				{
					if(TileGrid[Pair.Value]->TypeIndex == TileGrid[Pair.Key]->TypeIndex)
					{
						if(matchCount.Contains(TileGrid[Pair.Value]->TypeIndex))
							matchCount[TileGrid[Pair.Value]->TypeIndex]++;
						else
						{
							matchCount.Add(TileGrid[Pair.Value]->TypeIndex, 1);
						}
					}
				}
			}
		}

		for (const auto& element : matchCount)
		{
			if (element.Value >= 3)
			{
				return true;
			}
		}		
	}
	return false;
}

void ATileGrid::SearchMatchingTiles()
{
	MatchingTiles.Empty();
	
	for(int index = 0; index < TileGrid.Num(); index++)
	{
		//가로방향체크
		if(TileGrid.IsValidIndex(index) && TileGrid.IsValidIndex(index+1) && TileGrid.IsValidIndex(index+2))
		{
			if(TileGrid[index]->IsMatching(TileGrid[index+1].Get()) && TileGrid[index]->IsMatching(TileGrid[index+2].Get()))
			{
				if(MatchingTiles.Find(TileGrid[index].Get()) == INDEX_NONE)
					MatchingTiles.Add(TileGrid[index].Get());
				if(MatchingTiles.Find(TileGrid[index+1].Get()) == INDEX_NONE)
					MatchingTiles.Add(TileGrid[index+1].Get());
				if(MatchingTiles.Find(TileGrid[index+2].Get()) == INDEX_NONE)
					MatchingTiles.Add(TileGrid[index+2].Get());
			}
		}

		//세로방향체크
		if(TileGrid.IsValidIndex(index) && TileGrid.IsValidIndex(index+GridColumn) && TileGrid.IsValidIndex(index+GridColumn*2))
		{
			if(TileGrid[index]->IsMatching(TileGrid[index+GridColumn].Get()) && TileGrid[index]->IsMatching(TileGrid[index+GridColumn*2].Get()))
			{
				if(MatchingTiles.Find(TileGrid[index].Get()) == INDEX_NONE)
					MatchingTiles.Add(TileGrid[index].Get());
				if(MatchingTiles.Find(TileGrid[index+GridColumn].Get()) == INDEX_NONE)
					MatchingTiles.Add(TileGrid[index+GridColumn].Get());
				if(MatchingTiles.Find(TileGrid[index+GridColumn*2].Get()) == INDEX_NONE)
					MatchingTiles.Add(TileGrid[index+GridColumn*2].Get());
			}
		}
	}
}

void ATileGrid::ProcessMatchingTiles()
{
	for(TObjectPtr<ATile> tile : MatchingTiles)
	{
		int32 tileIndex = GetTileIndex(tile);
		if(tileIndex != INDEX_NONE)
		{
			TileGrid[tileIndex] = nullptr;
		}			
		UnusedTiles.Push(tile);
		tile->SetVisible(false);
	}
	MatchingTiles.Empty();
}

void ATileGrid::MoveTiles()
{
	for(int row = 1; row < GridRow; row++)
	{
		for(int col = 0; col < GridColumn; col++)
		{
			int tileIndex = GetTileIndexFromGridIndex(row,col);
			int targetIndex = GetTileIndexFromGridIndex(row-1,col);
			if(TileGrid[tileIndex] != nullptr && TileGrid[targetIndex] == nullptr)
			{
				TileGrid[targetIndex] = TileGrid[tileIndex];
				TileGrid[targetIndex]->SetActorRelativeLocation(FVector(0, col*TileWidth, (row-1)*TileHeight));				
				TileGrid[tileIndex] = nullptr;
			}
		}
	}
}

void ATileGrid::FillGrid()
{
	for(int index = (GridRow-1) * GridColumn; index < GridRow * GridColumn; index++)
	{
		if(TileGrid[index] == nullptr)
		{
			int8 typeIndex = FMath::RandRange(0, Materials.Num()-1);
			ATile* tile = UnusedTiles.Pop();
			if(tile)
			{
				tile->SetActorRelativeLocation(FVector(0, GetGridColumnFromTileIndex(index)*TileWidth, GetGridRowIndexFromTileIndex(index)*TileHeight));
				tile->SetTile(typeIndex, Materials[typeIndex]);
				TileGrid[index] = tile;
				tile->SetVisible(true);
			}
		}
	}
}

int32 ATileGrid::GetTileIndexFromGridIndex(int8 rowIndex, int8 columnIndex)
{
	if(rowIndex < 0 || rowIndex >= GridRow)
		return INDEX_NONE;

	if(columnIndex < 0 || columnIndex >= GridColumn)
		return INDEX_NONE;
	
	return rowIndex * GridColumn + columnIndex;
}

int8 ATileGrid::GetGridRowIndexFromTileIndex(int tileIndex)
{
	if(tileIndex < 0 || tileIndex >= GridRow*GridColumn)
		return INDEX_NONE;
		
	return tileIndex / GridColumn;
}

int8 ATileGrid::GetGridColumnFromTileIndex(int tileIndex)
{
	if(tileIndex < 0 || tileIndex >= GridRow*GridColumn)
		return INDEX_NONE;
	
	return tileIndex % GridColumn;
}

int32 ATileGrid::GetTileIndex(ATile* tile)
{
	return TileGrid.Find(tile);
}

bool ATileGrid::HasEmpty()
{
	return UnusedTiles.Num() > 0;
}

bool ATileGrid::IsSwapAble(ATile* tile1, ATile* tile2)
{
	int32 tile1Index = GetTileIndex(tile1);
	int32 tile2Index = GetTileIndex(tile2);

	if(tile1Index == INDEX_NONE || tile2Index == INDEX_NONE)
		return false;
	
	if(FMath::Abs(tile1Index - tile2Index) == 1 || FMath::Abs(tile1Index - tile2Index) == GridColumn)
		return true;

	return false;
}

void ATileGrid::UndoSwapProcess(ATile* tile1, ATile* tile2)
{
	UE_LOG(LogTemp, Display, TEXT("ATileGrid::UndoSwapProcess : %s, %s"),*tile1->GetName(),*tile2->GetName());
	if(IsSwapAble(tile1, tile2))
	{
		int32 tile1Index = GetTileIndex(tile1);
		int32 tile2Index = GetTileIndex(tile2);

		if(tile1Index == INDEX_NONE || tile2Index == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("UndoSwapProcess TileIndex None.  %s : %d, %s : %d"), *tile1->GetName(), tile1Index, *tile2->GetName(), tile2Index);
			return;
		}
		TileGrid[tile1Index] = tile2;
		TileGrid[tile2Index] = tile1;

		FVector loc1 = tile1->GetActorLocation();
		tile1->SetActorLocation(tile2->GetActorLocation());
		tile2->SetActorLocation(loc1);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile : These are non-continuous tiles."));
	}	
}

void ATileGrid::SwapProcess(ATile* tile1, ATile* tile2)
{
	UE_LOG(LogTemp, Display, TEXT("ATileGrid::SwapProcess : %s, %s"),*tile1->GetName(),*tile2->GetName());
	if(IsSwapAble(tile1, tile2))
	{
		int32 tile1Index = GetTileIndex(tile1);
		int32 tile2Index = GetTileIndex(tile2);

		if(tile1Index == INDEX_NONE || tile2Index == INDEX_NONE)
		{
			UE_LOG(LogTemp, Warning, TEXT("UndoSwapProcess TileIndex None.  %s : %d, %s : %d"), *tile1->GetName(), tile1Index, *tile2->GetName(), tile2Index);
			return;
		}

		TileGrid[tile1Index] = tile2;
		TileGrid[tile2Index] = tile1;
		
		FVector loc1 = tile1->GetActorLocation();
		tile1->SetActorLocation(tile2->GetActorLocation());
		tile2->SetActorLocation(loc1);
		
		int processedTileCount = 0;
		do
		{
			while(HasEmpty())
			{
				MoveTiles();
				FillGrid();
				
			}
			SearchMatchingTiles();
			ProcessMatchingTiles();
			processedTileCount += UnusedTiles.Num();
		}
		while (HasEmpty());

		
		if(processedTileCount == 0)
		{
			//Undo
			UndoSwapProcess(tile1, tile2);
		}
		else
		{
			//점수반영
		}

		if(IsMatchPossible() == false)
		{
			UE_LOG(LogTemp, Display, TEXT("GAME OVER : matching Impossible!"));
		}

	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile : These are non-continuous tiles."));
	}	
}