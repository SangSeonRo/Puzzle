// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GI_Puzzle.h"
#include "TileGrid.h"
#include "GameFramework/GameModeBase.h"
#include "GMB_Puzzle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API AGMB_Puzzle : public AGameModeBase
{
	GENERATED_BODY()
	AGMB_Puzzle();

protected:
	virtual void BeginPlay() override;

private:
	ATileGrid* TileGrid;
	UGI_Puzzle* GI_Puzzle;
};
