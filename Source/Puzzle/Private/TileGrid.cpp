// Fill out your copyright notice in the Description page of Project Settings.


#include "TileGrid.h"

#include "Tile.h"
#include "Components/SceneComponent.h"

// Sets default values
ATileGrid::ATileGrid()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TileClass = ATile::StaticClass();
	SetMaterials();

    USceneComponent* RootSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = RootSceneComponent;
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

void ATileGrid::InitializeTileGrid(int32 gridRow, int32 gridColumn)
{
	GridRow = gridRow;
	GridColumn = gridColumn;
	
	TileGridDestroyAll();
	MakeTileGrid();
}

void ATileGrid::TileGridDestroyAll()
{
	//그리드에 쓰인 타일 삭제.
	if( TileGrid.Num() > 0 )
	{
		for(int i = 0; i < TileGrid.Num(); i++)
		{
			TileGrid[i]->Destroy();					
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

void ATileGrid::MakeTileGrid()
{
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
		int8 gridRowIndex = GetGridRowIndexFromTileIndex(index);
		int8 gridColumnIndex = GetGridColumnFromTileIndex(index);

		//TMap<TileType,Count>
		TMap<int32,int32> tempIndexCount;
		tempIndexCount.Empty();
		//가로방향 체크
		for(int i = 0; i < 3 ;i++)
		{
			int tileIndex = GetTileIndexFromGridIndex(gridRowIndex, gridColumnIndex+i);
			if(tileIndex != INDEX_NONE)
			{
				if (tempIndexCount.Contains(TileGrid[tileIndex].Get()->TypeIndex))
				{
					tempIndexCount[TileGrid[tileIndex].Get()->TypeIndex]++;
				}
				else
				{
					tempIndexCount.Add(TileGrid[tileIndex].Get()->TypeIndex, 1);
				}
			}
		}
	    //동일 타일이 2개 이상이면 가능성을체크함.
		for (const auto& Elem : tempIndexCount)
		{
			if (Elem.Value > 1)
			{
				for(int i = 0; i < 3 ;i++)
				{
					int tileIndex = GetTileIndexFromGridIndex(gridRowIndex, gridColumnIndex+i);
					if(TileGrid[tileIndex].Get()->TypeIndex != Elem.Key)
					{
						switch(i)
						{
							case 0:
								{
									int tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex, gridColumnIndex+i-1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+1, gridColumnIndex+i);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex-1, gridColumnIndex+i);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}									
								}								
								break;
							case 1:
								{
									int tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+1, gridColumnIndex+i);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex-1, gridColumnIndex+i);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}									
								}
								break;
							case 2:
								{
									int tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex, gridColumnIndex+i+1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+1, gridColumnIndex+i);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex-1, gridColumnIndex+i);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}									
								}
								break;
						}
					}					
				}
				
				if (Elem.Value >= 3)
					return true;
			}
		}

		tempIndexCount.Empty();
		//세로방향 체크
		for(int i = 0; i < 3 ;i++)
		{
			int tileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex);
			if(tileIndex != INDEX_NONE)
			{
				if (tempIndexCount.Contains(TileGrid[tileIndex].Get()->TypeIndex))
				{
					tempIndexCount[TileGrid[tileIndex].Get()->TypeIndex]++;
				}
				else
				{
					tempIndexCount.Add(TileGrid[tileIndex].Get()->TypeIndex, 1);
				}
			}
		}
	    //동일 타일이 2개 이상이면 가능성을체크함.
		for (const auto& Elem : tempIndexCount)
		{
			if (Elem.Value > 1)
			{
				for(int i = 0; i < 3 ;i++)
				{
					int tileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex);
					if(TileGrid[tileIndex].Get()->TypeIndex != Elem.Key)
					{
						switch(i)
						{
							case 0: //좌우하
								{
									int tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex-1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex+1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex-1, gridColumnIndex);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}									
								}								
								break;
							case 1: //좌우
								{
									int tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex-1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex-i, gridColumnIndex+1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}									
								}
								break;
							case 2: //좌우상
								{
									int tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex-1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+i, gridColumnIndex+1);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}

									tempTileIndex = GetTileIndexFromGridIndex(gridRowIndex+i+1, gridColumnIndex);
									if(tempTileIndex != INDEX_NONE)
									{
										if (tempIndexCount.Contains(TileGrid[tempTileIndex].Get()->TypeIndex))
										{
											tempIndexCount[TileGrid[tempTileIndex].Get()->TypeIndex]++;
										}
									}									
								}
								break;
						}
					}					
				}
				
				if (Elem.Value >= 3)
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
		int8 gridRowIndex = GetGridRowIndexFromTileIndex(index);
		int8 gridColumnIndex = GetGridColumnFromTileIndex(index);

		if(gridRowIndex > GridRow - 3)
			continue;

		if(gridColumnIndex > GridColumn - 3)
			continue;

		//가로방향매칭타일 체크.
		TArray<TObjectPtr<ATile>> horizontalMatchTiles;
		int tempColumnIndex = gridColumnIndex;
		while(true)
		{
			++tempColumnIndex;
			if(tempColumnIndex >= GridColumn)
				break;

			int targetIndex = GetTileIndexFromGridIndex(gridRowIndex,tempColumnIndex);

			if(TileGrid[targetIndex] == nullptr)
				break;

			if(TileGrid[index]->IsMatching(TileGrid[targetIndex].Get()))
				horizontalMatchTiles.Add(TileGrid[targetIndex].Get());
			else
				break;
		}
		if(horizontalMatchTiles.Num()>=2)
		{
			if(MatchingTiles.Find(TileGrid[index].Get()) == INDEX_NONE)
				MatchingTiles.Add(TileGrid[index].Get());
			for(int i = 0; i < horizontalMatchTiles.Num(); i++)
			{
				if(MatchingTiles.Find(horizontalMatchTiles[i]) == INDEX_NONE)
					MatchingTiles.Add(horizontalMatchTiles[i]);
			}
		}

		//세로방향매칭타일체크
		TArray<TObjectPtr<ATile>> verticalMatchTiles;
		int tempRowIndex = gridRowIndex;
		while(true)
		{
			++tempRowIndex;
			if(tempRowIndex >= GridRow)
				break;

			int targetIndex = GetTileIndexFromGridIndex(tempRowIndex,gridColumnIndex);

			if(TileGrid[targetIndex] == nullptr)
				break;

			if(TileGrid[index]->IsMatching(TileGrid[targetIndex].Get()))
				verticalMatchTiles.Add(TileGrid[targetIndex].Get());
			else
				break;
		}
		if(verticalMatchTiles.Num()>=2)
		{
			if(MatchingTiles.Find(TileGrid[index].Get()) == INDEX_NONE)
				MatchingTiles.Add(TileGrid[index].Get());
			for(int i = 0; i < verticalMatchTiles.Num(); i++)
			{
				if(MatchingTiles.Find(verticalMatchTiles[i]) == INDEX_NONE)
					MatchingTiles.Add(verticalMatchTiles[i]);
			}	
		}
	}
}

void ATileGrid::ProcessMatchingTiles()
{
	for(TObjectPtr<ATile> tile : MatchingTiles)
	{
		int tileIndex = GetTileIndex(tile);
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
				TileGrid[targetIndex]->SetActorLocation(FVector(0, col*TileWidth, (row-1)*TileHeight));				
				TileGrid[tileIndex] = nullptr;
			}
		}
	}
}

void ATileGrid::FillGrid()
{
	for(int index = GridRow * (GridColumn-1); index < GridRow * GridColumn; index++)
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

int ATileGrid::GetTileIndexFromGridIndex(int8 rowIndex, int8 columnIndex)
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

int ATileGrid::GetTileIndex(ATile* tile)
{
	return TileGrid.Find(tile);
}

bool ATileGrid::HasEmpty()
{
	return UnusedTiles.Num() > 0;
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
		
		TileGrid[tile1Index] = tile2;
		TileGrid[tile2Index] = tile1;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile : These are non-continuous tiles."));
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
		
		TileGrid[tile1Index] = tile2;
		TileGrid[tile2Index] = tile1;
		
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
			UndoSwapTile(tile1, tile2);
		}
		else
		{
			//점수반영

			//매칭되는 타일이 없으면 게임종료.
			if(IsMatchPossible() == false)
			{
			
			}
		}		
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Can't SwapTile : These are non-continuous tiles."));
	}	
}