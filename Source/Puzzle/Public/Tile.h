// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

class UBoxComponent;

UCLASS()
class PUZZLE_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();
	void SetTile(int8 typeIndex, UMaterialInterface* material);
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	int32 TypeIndex;
	
public:
	bool isMatching(ATile* otherTile);
	void Focusing(bool isFocus);
};
