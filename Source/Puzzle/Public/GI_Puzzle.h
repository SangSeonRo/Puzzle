// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GI_Puzzle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API UGI_Puzzle : public UGameInstance
{
	GENERATED_BODY()

private:
	int32 _CurrentScore;
	int32 _RemainingMoves;

public:
	void Reset(){
		_CurrentScore = 0;
		_RemainingMoves = 30;
	}
	
	void AddScore(int32 addScore){ _CurrentScore+=addScore;	}
	void DecreaseMoves(){ _RemainingMoves--; }
	int32 CurrentScore() const { return _CurrentScore; }
	int32 RemainingMoves() const { return _RemainingMoves; }
};
