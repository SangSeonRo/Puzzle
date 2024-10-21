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
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	UBoxComponent* BoxComponent;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Tile", meta = (AllowPrivateAccess = "true"))
	int32 TypeIndex;
	bool IsMatching(ATile* OtherTile) const;
	void SetTile(int8 typeIndex, UMaterialInterface* material);
	void Selected(bool isSelected);

	void SetVisible(bool visible);
};
