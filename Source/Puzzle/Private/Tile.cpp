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

bool ATile::IsMatching(ATile* OtherTile) const
{
	if(OtherTile == nullptr)
		return false;
	
	return TypeIndex ==  OtherTile->TypeIndex;
}

void ATile::SetTile(int8 typeIndex, UMaterialInterface* material)
{
	TypeIndex = typeIndex;
	StaticMesh->SetMaterial(0, material);
}

void ATile::Selected(bool isSelected)
{
	if(StaticMesh == nullptr)
		return;

	//포커싱되면 스케일변경.
	if(isSelected)
	{
		StaticMesh->SetRelativeScale3D(FVector(0.8f));
	}
	else
	{
		StaticMesh->SetRelativeScale3D(FVector(0.95f));
	}
}

void ATile::SetVisible(bool visible)
{
	if(StaticMesh == nullptr)
		return;

	StaticMesh->SetVisibility(visible);
}
