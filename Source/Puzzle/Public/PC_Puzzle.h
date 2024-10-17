// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "Tile.h"
#include "PC_Puzzle.generated.h"

/**
 * 
 */
class ICommand;
UCLASS()
class PUZZLE_API APC_Puzzle : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void SetupInputComponent() override;
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Blueprintable, Category="Input")
	UInputMappingContext* DefaultMappingContext;

	UPROPERTY(EditAnywhere, Blueprintable, Category="Input")
	UInputAction* IA_Select;
	
	void InputSelect(const FInputActionValue& Value);
	void ExcuteCommand();
	void UndoLastCommand();
	
private:
	
	void Select(AActor* Tile);
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ATile*> SelectTiles;
	
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<ICommand*> CommandHistory;


};
