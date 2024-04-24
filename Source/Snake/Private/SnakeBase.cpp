// Fill out your copyright notice in the Description page of Project Settings.


#include "SnakeBase.h"
#include "../Public/SnakeElementBase.h"
#include "Interactable.h"

// Sets default values
ASnakeBase::ASnakeBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ElementSize = 120;
}

// Called when the game starts or when spawned
void ASnakeBase::BeginPlay()
{
	Super::BeginPlay();
	SetActorTickInterval(MovementSpeed);
	AddSnakeElement(4);
	LastMoveDirection = EMovementDirection::M_DOWN;
		
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
		FVector NewLocation(FVector::ZeroVector);

		if (int32 Index = SnakeElements.Num() > 0)
		{
			ASnakeElementBase* PrevElement = SnakeElements[Index - 1];
			FVector PrevLocation = PrevElement->GetActorLocation();


			switch (LastMoveDirection)
			{
			case EMovementDirection::M_UP:
				NewLocation += ElementSize * PrevLocation;
				break;

			case EMovementDirection::M_DOWN:
				NewLocation -= ElementSize * PrevLocation;
				break;

			case EMovementDirection::M_LEFT:
				NewLocation += ElementSize * PrevLocation;
				break;

			case EMovementDirection::M_RIGHT:
				NewLocation -= ElementSize * PrevLocation;
				break;
			}
		}

		FTransform NewTransform(NewLocation);
		ASnakeElementBase* NewSnakeElem = GetWorld()->SpawnActor<ASnakeElementBase>(SnakeElementClass, NewTransform);
		if (NewSnakeElem)
		{
			NewSnakeElem->SnakeOwner = this;
			NewSnakeElem->SetColor(FLinearColor::Red);
			int32 ElemIndex = SnakeElements.Add(NewSnakeElem);
			if (ElemIndex == 0)
			{
				NewSnakeElem->SetFirstElementType();
			}
		}	
	}	
}

void ASnakeBase::Move()
{
	FVector MovementVector(FVector::ZeroVector);

		switch (LastMoveDirection)
		{
		case EMovementDirection::M_UP:
			MovementVector.X += ElementSize;
			break;
		case EMovementDirection::M_DOWN:
			MovementVector.X -= ElementSize;
			break;
		case EMovementDirection::M_LEFT:
			MovementVector.Y += ElementSize;
			break;
		case EMovementDirection::M_RIGHT:
			MovementVector.Y -= ElementSize;
			break;
		}

	SnakeElements[0]->ToggleCollision();

	for (int i = SnakeElements.Num() - 1; i > 0; --i)
	{
		auto CurrentElement = SnakeElements[i];
		auto PrevElement = SnakeElements[i - 1];
		FVector PrevLocation = PrevElement->GetActorLocation();
		CurrentElement->SetActorLocation(PrevLocation);
	}
	
	SnakeElements[0]->AddActorWorldOffset(MovementVector);
	SnakeElements[0]->ToggleCollision();

	HasMoved = false;
}

void ASnakeBase::SnakeElementOverlap(ASnakeElementBase* OverlappedElement, AActor* Other)
{
	if (IsValid(OverlappedElement))
	{
		int32 ElemIndex;
		SnakeElements.Find(OverlappedElement, ElemIndex);
		bool bIsFirst = ElemIndex == 0;
		IInteractable* InteractableInterface = Cast<IInteractable>(Other);
		if (InteractableInterface)
		{
			InteractableInterface->Interact(this, bIsFirst);
		}
	}
}

