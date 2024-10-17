// Fill out your copyright notice in the Description page of Project Settings.


#include "GMB_Puzzle.h"
#include "TileGrid.h"
#include "Kismet/GameplayStatics.h"

void AGMB_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	// UGI_Puzzle* GameInstance = Cast<UGI_Puzzle>(UGameplayStatics::GetGameInstance(GetWorld()));
	// if(GameInstance)
	// {
	// 	GameInstance->ResetGameState();
	// }

	//ObserverGameState = NewObject<UGameStateSubject>();

	if(mainWidget)
	{
		mainWidget->AddToViewport();
		//ObserverGameState->RegisterObserver(mainWidget);
		//ObserverGameState->Init();
	}
	
	ATileGrid* TileGrid = Cast<ATileGrid>(UGameplayStatics::GetActorOfClass(GetWorld(),ATileGrid::StaticClass()));
	if(TileGrid)
	{
		TileGrid->MakeGrid();
	}
}
