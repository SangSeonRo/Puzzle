// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tile.h"
#include "Engine/GameInstance.h"
#include "GI_Puzzle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API UGI_Puzzle : public UGameInstance
{
	GENERATED_BODY()

public:
	UGI_Puzzle();
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int32 PlayerScore;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Game Data")
	int32 RemainingMoves;

	UFUNCTION(BlueprintCallable, Category = "Game Functions")
	void AddScore(int32 points);

	UFUNCTION(BlueprintCallable, Category = "Game Functions")
	void DecreaseMoves();

	UFUNCTION(BlueprintCallable, Category = "Game Functions")
	void ResetGameState();
};
