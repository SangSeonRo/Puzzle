// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Command.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UCommand : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class PUZZLE_API ICommand
{
	GENERATED_BODY()

public:
	virtual void Execute() = 0;
	virtual void Undo() = 0;
};
