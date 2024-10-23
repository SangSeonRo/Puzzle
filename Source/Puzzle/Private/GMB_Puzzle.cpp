// Fill out your copyright notice in the Description page of Project Settings.


#include "GMB_Puzzle.h"

#include "GI_Puzzle.h"
#include "PC_Puzzle.h"
#include "TileGrid.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/DirectionalLight.h"
#include "Kismet/GameplayStatics.h"

AGMB_Puzzle::AGMB_Puzzle()
{
	GridRow = 6;
	GridColumn = 6;
}

void AGMB_Puzzle::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* pc = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if(pc)
	{
		PC_Puzzle = Cast<APC_Puzzle>(pc);
	}
	
	SetupDirectionalLight();
	SetupCamera();

	TSubclassOf<ATileGrid> tileGridClass = ATileGrid::StaticClass();
	TileGrid = GetWorld()->SpawnActor<ATileGrid>(tileGridClass);
	if(TileGrid == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("TileGrid Spawn Failed."));
		return;
	}

	GI_Puzzle = Cast<UGI_Puzzle>(GetGameInstance());
	if(GI_Puzzle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GI_Puzzle Not Found."));
		return;
	}
	TileGrid->SetMaterials(Materials);
	TileGrid->InitializeTileGrid(GridRow,GridColumn);
}

void AGMB_Puzzle::SetupCamera()
{
	if (UWorld* World = GetWorld())
	{
		// 카메라 액터 스폰
		ACameraActor* CameraActor = World->SpawnActor<ACameraActor>(FVector::ZeroVector, FRotator::ZeroRotator);
		if (CameraActor)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Camera Actor spawned successfully!"));
	
			// 필요에 따라 카메라 설정을 조정할 수 있음
			CameraActor->SetActorLabel(TEXT("CameraActor"));
			CameraComponent = CameraActor->GetCameraComponent();

			CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
			CameraComponent->SetOrthoWidth(1920);
			CameraComponent->SetAutoActivate(true);

			if(PC_Puzzle)
				PC_Puzzle->SetViewTarget(CameraActor);
		}
	}
}

// 디렉셔널 라이트 액터를 월드에 스폰하는 함수
void AGMB_Puzzle::SetupDirectionalLight()
{
	if (UWorld* World = GetWorld())
	{
		// 스폰할 디렉셔널 라이트 위치와 회전
		FVector LightLocation = FVector(0.0f, 0.0f, 0.0f);  // 대체로 위치는 상관없음, 빛의 방향만 중요
		FRotator LightRotation = FRotator(0.0f, 0.0f, 0.0f);

		// 디렉셔널 라이트 액터 스폰
		ADirectionalLight*  DirectionalLight = World->SpawnActor<ADirectionalLight>(LightLocation, LightRotation);
		if (DirectionalLight)
		{
			//UE_LOG(LogTemp, Warning, TEXT("Directional Light Actor spawned successfully!"));
			
			// 필요에 따라 라이트 설정을 조정할 수 있음
			DirectionalLight->SetActorLabel(TEXT("MyDirectionalLight"));
			DirectionalLightComponent = DirectionalLight->GetComponent();
			DirectionalLightComponent->SetIntensity(20);
		}
	}
}