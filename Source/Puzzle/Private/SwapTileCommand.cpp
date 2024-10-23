// Fill out your copyright notice in the Description page of Project Settings.


#include "SwapTileCommand.h"
#include "TileGrid.h"

void USwapTileCommand::Initalize(ATileGrid* tileGrid, ATile* tile0, ATile* tile1)
{
	TileGrid = tileGrid;
	Tile0 = tile0;
	Tile1 = tile1;
}

void USwapTileCommand::Execute()
{
	TileGrid->SwapProcess(Tile0, Tile1);
}

void USwapTileCommand::Undo()
{
	TileGrid->UndoSwapProcess(Tile0, Tile1);
}
