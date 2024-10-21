// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/BoxComponent.h"

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	SetRootComponent(BoxComponent);
	BoxComponent->SetBoxExtent(FVector(50.0f));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(BoxComponent);
	StaticMesh->SetRelativeScale3D(FVector(0.95f));

	ConstructorHelpers::FObjectFinder<UStaticMesh> InitMesh(TEXT("/Script/Engine.StaticMesh'/Engine/BasicShapes/Cube.Cube'"));
	if (InitMesh.Succeeded())
	{
		StaticMesh->SetStaticMesh(InitMesh.Object);
	}	
}

void ATile::SetTile(int8 typeIndex, UMaterialInterface* material)
{
	TypeIndex = typeIndex;
	StaticMesh->SetMaterial(0, material);
}

bool ATile::isMatching(ATile* otherTile)
{
	if (otherTile == nullptr)
		return false;
	
	return TypeIndex == otherTile->TypeIndex;
}

void ATile::Focusing(bool isFocus)
{
	if(StaticMesh == nullptr)
		return;

	//포커싱되면 스케일변경.
	if(isFocus)
	{
		StaticMesh->SetRelativeScale3D(FVector(0.8f));
	}
	else
	{
		StaticMesh->SetRelativeScale3D(FVector(0.95f));
	}
}