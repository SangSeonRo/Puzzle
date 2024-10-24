// Fill out your copyright notice in the Description page of Project Settings.


#include "Tile.h"

#include "Components/BoxComponent.h"

void ATile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

// Sets default values
ATile::ATile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	StaticMesh->SetVisibility(visible);
}

void ATile::MoveTile(FVector TargetLocation, float Duration)
{
	FVector StartLocation = GetActorLocation();
	FTimerHandle MoveTimerHandle;
	float ElapsedTime = 0.0f;

	IsMoving = true;

	UE_LOG(LogTemp, Warning, TEXT("Moving Tile : %s, ( %f, %f, %f )"),*this->GetName() , StartLocation.X , StartLocation.Y , StartLocation.Z);
	GetWorld()->GetTimerManager().SetTimer(MoveTimerHandle, FTimerDelegate::CreateLambda([this, StartLocation, TargetLocation, &MoveTimerHandle, &ElapsedTime, Duration]()
	{
		ElapsedTime += GetWorld()->GetDeltaSeconds();
		float Alpha = FMath::Clamp(ElapsedTime / Duration, 0.0f, 1.0f);
		FVector NewLocation = FMath::Lerp(StartLocation, TargetLocation, Alpha);
		UE_LOG(LogTemp, Warning, TEXT("Moving Tile : %s, ( %f, %f, %f ), ElapsedTime : %f"),*this->GetName() , NewLocation.X , NewLocation.Y , NewLocation.Z, ElapsedTime);
		SetActorRelativeLocation(NewLocation);

		if (Alpha >= 1.0f)
		{
			IsMoving = false;
			GetWorld()->GetTimerManager().ClearTimer(MoveTimerHandle);
		}
	}), 0.01f, true);
}
