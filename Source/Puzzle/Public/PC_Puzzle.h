// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PC_Puzzle.generated.h"

/**
 * 
 */
UCLASS()
class PUZZLE_API APC_Puzzle : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;
	void PerformLineTrace();
};
