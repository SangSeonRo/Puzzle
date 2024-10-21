// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileGrid.generated.h"

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

	TArray<TArray<TWeakObjectPtr<ATile>>> TileGrid;
	TArray<TObjectPtr<ATile>> UnusedTiles;

	void DestroyAllTiles();
	void MakeGrid();
public:
	void InitializeTileGrid(int32 gridRow, int32 gridColumn);
	FVector GetCameraPosition();
};
