// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "Tile.h"

/**
 * 
 */
class PUZZLE_API SwapTilesCommand : public ICommand
{
private:
	ATile* FirstTile;
	ATile* SecondTile;
	int8 FirstTileTypeIndex;
	int8 SecondTileTypeIndex;
	
public:
	SwapTilesCommand(ATile* InFirstTile, ATile* InSecondTile);
	
	virtual void Execute() override;

	virtual void Undo() override;		
};
