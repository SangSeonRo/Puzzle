// Fill out your copyright notice in the Description page of Project Settings.


#include "PC_Puzzle.h"

void APC_Puzzle::SetupInputComponent()
{
	Super::SetupInputComponent();
}

void APC_Puzzle::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	SetInputMode(InputMode);
}

void APC_Puzzle::PerformLineTrace()
{
	FVector WorldLocation, WorldDirection;
	if (DeprojectMousePositionToWorld(WorldLocation, WorldDirection))
	{
		FVector Start = WorldLocation;
		FVector End = Start + (WorldDirection * 10000.0f);

		FHitResult HitResult;
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);

		if (HitResult.bBlockingHit)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *HitResult.GetActor()->GetName());
		}
	}
}