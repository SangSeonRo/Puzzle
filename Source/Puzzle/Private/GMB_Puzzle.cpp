// Fill out your copyright notice in the Description page of Project Settings.


#include "GMB_Puzzle.h"

#include "Kismet/GameplayStatics.h"

AGMB_Puzzle::AGMB_Puzzle()
{
	TileGrid = NewObject<ATileGrid>();
}

void AGMB_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	TileGrid->MakeGrid();
}
