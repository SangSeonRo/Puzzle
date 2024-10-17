// Fill out your copyright notice in the Description page of Project Settings.

#include "SwapTilesCommand.h"
#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

SwapTilesCommand::SwapTilesCommand(ATile* InFirstTile, ATile* InSecondTile)
{
	FirstTile = InFirstTile;
	SecondTile = InSecondTile;
}

void SwapTilesCommand::Execute()
{
	UE_LOG(LogTemp, Warning, TEXT("SwapTilesCommand::Execute"));
	auto temp = UGameplayStatics::GetActorOfClass(FirstTile->GetWorld(), ATileGrid::StaticClass());
	ATileGrid* tileGrid = Cast<ATileGrid>(temp);
	if(tileGrid)
	{
		tileGrid->SwapTile(FirstTile, SecondTile);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapTilesCommand::ATileGrid Null"));
	}
}

void SwapTilesCommand::Undo()
{
	UE_LOG(LogTemp, Warning, TEXT("SwapTilesCommand::Undo"));
	auto temp = UGameplayStatics::GetActorOfClass(FirstTile->GetWorld(), ATileGrid::StaticClass());
	ATileGrid* tileGrid = Cast<ATileGrid>(temp);
	if(tileGrid)
	{
		tileGrid->UndoSwapTile(FirstTile, SecondTile);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("SwapTilesCommand::ATileGrid Null"));
	}
}