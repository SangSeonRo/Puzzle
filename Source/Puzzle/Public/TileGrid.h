// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "GameFramework/Actor.h"
#include "TileGrid.generated.h"

UCLASS()
class PUZZLE_API ATileGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGrid();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	int8 GridRow = 6;
	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	int8 GridColumn = 6;
	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	TSubclassOf<ATile> TileClass;
	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	TArray<UMaterialInterface*> Materials;
	
	UPROPERTY(VisibleAnywhere, Category = "Grid")
	TArray<ATile*> Tiles;

	void MakeGrid();
	void FillGrid();
	TArray<ATile*> SearchMatchingTile(ATile* tile);
	void MatchingTileDestroy();
	bool IsSwapAble(ATile* tile1, ATile* tile2);
	void SwapTile(ATile* tile1, ATile* tile2);
	void MoveTiles();
	int GetTileIndexFromGridIndex(int8 row, int8 column);
	int8 GetGridRowIndexFromTileIndex(int tileIndex);
	int8 GetGridColumnFromTileIndex(int tileIndex);
	int GetTileIndex(ATile* tile);
	bool HasEmpty();
};