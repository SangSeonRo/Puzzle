// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "PC_Puzzle.generated.h"

UCLASS()
class PUZZLE_API APC_Puzzle : public APlayerController
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;
private:
	void InputAction_Mouse_BT_Left(const FInputActionValue& value);	
public:
	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputMappingContext* InputMappingContext;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
	UInputAction* IA_Mouse_BT_Left;
};
