// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GMB_Puzzle.generated.h"

/**
 * 
 */
class UCameraComponent;
class UDirectionalLightComponent;
class APC_Puzzle;
class ATileGrid;
class UGI_Puzzle;

UCLASS()
class PUZZLE_API AGMB_Puzzle : public AGameModeBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;
	void SetupCamera();
	void SetupDirectionalLight();

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Light", meta = (AllowPrivateAccess = "true"))
	UDirectionalLightComponent* DirectionalLightComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	APC_Puzzle* PC_Puzzle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ATileGrid* TileGrid;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UGI_Puzzle* GI_Puzzle;
};
