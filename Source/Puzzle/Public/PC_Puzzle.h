// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PC_Puzzle.generated.h"

class ATileGrid;
class ATile;

UCLASS()
class PUZZLE_API APC_Puzzle : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	void InputAction_Mouse_BT_Left(const FInputActionValue& value);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelectedTile", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ATile> SelectedTile0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelectedTile", meta = (AllowPrivateAccess = "true"))
	TWeakObjectPtr<ATile> SelectedTile1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SelectedTile", meta = (AllowPrivateAccess = "true"))
	ATileGrid* TileGrid;
public:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_Mouse_BT_Left;

	
};
