// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UCLASS()
class PUZZLE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	void SetTile(int8 typeIndex, int8 rowIndex, int8 columnIndex, UMaterialInterface* material);

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere)
	int8 TypeIndex = -1;
	UPROPERTY(VisibleAnywhere)
	int8 RowIndex = -1;
	UPROPERTY(VisibleAnywhere)
	int8 ColumnIndex = -1;
};