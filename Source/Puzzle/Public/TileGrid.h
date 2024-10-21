// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileGrid.generated.h"

constexpr float TileWidth = 100.0f;
constexpr float TileHeight = 100.0f;

class ATile;

UCLASS()
class PUZZLE_API ATileGrid : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATileGrid();
	void SetMaterials();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:	
	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	int8 GridRow = 6;

	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	int8 GridColumn = 6;

	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ATile> TileClass;
	
	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TArray<UMaterialInterface*> Materials;

	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TArray<TWeakObjectPtr<ATile>> TileGrid;

	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ATile>> UnusedTiles;

	UPROPERTY(VisibleAnywhere, Category = "Grid", meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ATile>> MatchingTiles;
	
	void TileGridDestroyAll();
	void MakeTileGrid();
	void SearchMatchingTiles();
	void ProcessMatchingTiles();
	void MoveTiles();
	void FillGrid();

public:
	void InitializeTileGrid(int32 gridRow, int32 gridColumn);
	int GetTileIndexFromGridIndex(int8 rowIndex, int8 columnIndex);
	int8 GetGridRowIndexFromTileIndex(int tileIndex);
	int8 GetGridColumnFromTileIndex(int tileIndex);
	int GetTileIndex(ATile* tile);
	bool HasEmpty();
	bool IsSwapAble(ATile* tile1, ATile* tile2);
	void UndoSwapTile(ATile* tile1, ATile* tile2);
	void SwapTile(ATile* tile1, ATile* tile2);
};
