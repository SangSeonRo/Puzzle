// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "SwapTileCommand.generated.h"

/**
 * 
 */
class ATileGrid;
class ATile;
UCLASS()
class PUZZLE_API USwapTileCommand : public UObject, public ICommand
{
	GENERATED_BODY()

private:
	ATileGrid* TileGrid;
	ATile* Tile0;
	ATile* Tile1;
public:
	void Initalize(ATileGrid* tileGrid, ATile* tile0, ATile* tile1);

	virtual void Execute() override;
	virtual void Undo() override;
};
