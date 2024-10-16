// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TileCommandInvoker.generated.h"

class ICommand;
UCLASS()
class PUZZLE_API ATileCommandInvoker : public AActor
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ICommand*> CommandHistory;
	
public:	
	// Sets default values for this actor's properties
	ATileCommandInvoker();

	void ExecuteCommand(ICommand* Command);
	void UndoLastCommand();
};
