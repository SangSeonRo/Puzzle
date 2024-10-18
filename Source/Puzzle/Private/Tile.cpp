// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATile::SetTile(int8 typeIndex, UMaterialInterface* material)
{
	TypeIndex = typeIndex;
	StaticMesh->SetMaterial(0, material);
}

bool ATile::isMatching(ATile* otherTile)
{
	if (otherTile == nullptr)
		return false;
	
	return TypeIndex == otherTile->TypeIndex;
}

void ATile::Focusing(bool isFocus)
{
	if(StaticMesh == nullptr)
		return;
	
	if(isFocus)
	{
		StaticMesh->SetRelativeScale3D(FVector(0.8f));
	}
	else
	{
		StaticMesh->SetRelativeScale3D(FVector(0.95f));
	}
}