// Fill out your copyright notice in the Description page of Project Settings.


#include "GI_Puzzle.h"

void UGI_Puzzle::SetSelectedTile(ATile* Tile)
{
	if (Tile != nullptr)
	{
		PreSelectedTile = SelectedTile;
		SelectedTile = Tile;
	}
	else
	{
		PreSelectedTile = SelectedTile = nullptr;
	}

	if(SelectedTile!=nullptr)
		UE_LOG(LogTemp, Display, TEXT("Select Tile : %s"), *SelectedTile->GetName());
	if(PreSelectedTile!=nullptr)
		UE_LOG(LogTemp, Display, TEXT("PreSelect Tile : %s"), *PreSelectedTile->GetName());
}