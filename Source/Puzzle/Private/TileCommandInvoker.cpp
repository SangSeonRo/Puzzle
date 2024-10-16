// Fill out your copyright notice in the Description page of Project Settings.


#include "TileCommandInvoker.h"

#include "Command.h"

// Sets default values
ATileCommandInvoker::ATileCommandInvoker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

void ATileCommandInvoker::ExecuteCommand(ICommand* Command)
{
	if (Command == nullptr)
		return;

	Command->Execute();
	CommandHistory.Push(Command);
}

void ATileCommandInvoker::UndoLastCommand()
{
	if(CommandHistory.Num() > 0)
	{
		CommandHistory.Last()->Undo();
		CommandHistory.Pop();		
	}
}
