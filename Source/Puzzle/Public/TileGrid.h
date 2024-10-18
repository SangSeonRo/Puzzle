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

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	int8 GridRow = 6;

	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	int8 GridColumn = 6;
	
	UPROPERTY(EditAnywhere, Category = "Grid Setting")
	TArray<UMaterialInterface*> Materials;
	
	TArray<TArray<ATile*>> TileGrid;

	void MakeGrid();
	
};
