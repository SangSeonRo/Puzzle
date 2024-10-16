// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTilesCommand.h"
#include "TileGrid.h"

SwapTilesCommand::SwapTilesCommand(ATile* InFirstTile, ATile* InSecondTile)
{
	FirstTile = InFirstTile;
	SecondTile = InSecondTile;
	FirstTileTypeIndex = FirstTile->TypeIndex;
	SecondTileTypeIndex = SecondTile->TypeIndex;
}

void SwapTilesCommand::Execute()
{
	
}

void SwapTilesCommand::Undo()
{
	
}
