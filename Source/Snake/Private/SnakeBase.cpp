// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "../Public/SnakeElementBase.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ElementSize = 100;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
	MovementSpeed = 50.f;
	LastMoveDirection = EMovementDirection::M_LEFT;
		
}

// Called every frame
void ASnakeBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Move();

}

void ASnakeBase::AddSnakeElement(int ElementsNum)
{
	for (int i = 0; i < ElementsNum; ++i)
	{
		FVector NewLocation(SnakeElements.Num() * ElementSize, 0, 0);
		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
		if (ElemIndex == 0)
		{
			NewSnakeElem->SetFirstElementType();
		}
	}	
}

void ASnakeBase::Move()
{
	FVector MovementVector(ForceInitToZero);
	MovementSpeed = ElementSize;

	switch (LastMoveDirection)
	{
	case EMovementDirection::M_UP:
		MovementVector.X += MovementSpeed;
		break;
	case EMovementDirection::M_DOWN:
		MovementVector.X -= MovementSpeed;
		break;
	case EMovementDirection::M_LEFT:
		MovementVector.Y += MovementSpeed;
		break;
	case EMovementDirection::M_RIGHT:
		MovementVector.Y -= MovementSpeed;
		break;
	}

	for (int i = SnakeElements.Num() - 1; i > 0; --i)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
}

