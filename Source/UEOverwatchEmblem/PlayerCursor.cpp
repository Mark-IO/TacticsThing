// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "MapGameStateBase.h"
#include "CursorActor.h"
#include "UnitPawn.h"
#include "PlayerCursor.h"



// Sets default values
APlayerCursor::APlayerCursor(const FObjectInitializer& ObjectInitializer)
{
 
	PrimaryActorTick.bCanEverTick = true;
	this->bShowMouseCursor = true;
	this->bEnableMouseOverEvents = true;
}

//called to bind input
void APlayerCursor::SetupInputComponent()
{
	// Set up gameplay key bindings
	Super::SetupInputComponent();
	InputComponent->BindAction("Confirm", IE_Pressed, this, &APlayerCursor::ProcessConfirm);
	InputComponent->BindAction("Cancel", IE_Pressed, this, &APlayerCursor::ProcessCancel);

	InputComponent->BindAction("ToggleDangerZone", IE_Pressed, this, &APlayerCursor::ProcessToggleDangerZone);
	
}

void APlayerCursor::ProcessConfirm()
{
	ClientMessage("Confirm Ran!");
	/*
		Get the cursor location and see if it is over anything
		If the cursor is over nothing bring up the turn menu,
		If the cursor is over a player unit, show its movement and attack range and begin drawing path
			-A secondary confirm moves the player across the path and then brings up the character menu for actions like attack etc.
			-From there it can go to attack or equipment interface to which the final confirm will follow through with the actions
		If the cursor is over an enemy or a special tile like ballista, highlight its range
	*/
	AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());
	if (state)
	{
		ACursorActor* cursor = Cast<ACursorActor>(AcknowledgedPawn);
		if (cursor)
		{
			if (state->GameplayState == EGameplayStateEnum::EG_BaseMenu)
			{
			
				if (cursor->CurrentTile->bIsOccupied)
				{
					AUnitPawn* pawn = AUnitPawn::GetUnitPawnAt(cursor->CurrentTile);
					if (pawn)
					{
						if (pawn->Faction == EFactionEnum::EF_Ally)
						{
							if (!pawn->HasMovedThisTurn())
							{
								//Add basemenu to the stack and set the new state to the next menu
								state->PreviousStates.Push(state->GameplayState);
								state->GameplayState = EGameplayStateEnum::EG_MoveMenu;
								AUnitPawn::SelectedUnitPawn = pawn;
							}
						}
						else if (pawn->Faction == EFactionEnum::EF_Enemy)
						{
							//Toggle the highlighted danger zone
							;
						}
					}
				}
				else
				{
					state->PreviousStates.Push(state->GameplayState);
					state->GameplayState = EGameplayStateEnum::EG_TurnMenu;
				}
			}

			else if (state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
			{
				if (AUnitPawn::CurrentPath.Num() == 0 || cursor->CurrentTile == AUnitPawn::CurrentPath.Last())
				{
					state->PreviousStates.Push(state->GameplayState);
					state->GameplayState = EGameplayStateEnum::EG_ActionMenu;
					
				}
			}
			
		}	
		
	}
	
}
void APlayerCursor::ProcessCancel()
{
	ClientMessage("Cancel Ran!");
	/*
		Backs out to the previous process/menu before the current one, does nothing at base
	*/
	AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());

	if (state->PreviousStates.Num() != 0)
	{
		state->GameplayState = state->PreviousStates.Pop();
		if (state->GameplayState == EGameplayStateEnum::EG_BaseMenu)
		{
			AUnitPawn::CurrentPath.Empty();
			AUnitPawn::SelectedUnitPawn = nullptr;
		}
		if (state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
		{
			FVector desiredLocation = AUnitPawn::SelectedUnitPawn->currentLocation->GetActorLocation() + FVector(0, 0, AUnitPawn::SelectedUnitPawn->GetActorLocation().Z);
			AUnitPawn::SelectedUnitPawn->SetActorLocation(desiredLocation);
			AUnitPawn::SelectedUnitPawn->ResetMoveCount();
		}
		if (state->GameplayState == EGameplayStateEnum::EG_ActionMenu)
		{

		}
	}
}

void APlayerCursor::ProcessToggleDangerZone()
{
	//Send a message to the cursor with camera to Zoom the camera in up until a minimum
	ClientMessage("DangerZone Ran!");

}

// Called when the game starts or when spawned
void APlayerCursor::BeginPlay()
{
	Super::BeginPlay();
	GetMousePosition(LastMousePosition.X, LastMousePosition.Y);
}

// Called every frame
void APlayerCursor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FVector2D MouseDelta;

	GetMousePosition(MouseDelta.X, MouseDelta.Y);
	MouseDelta = DeltaTime * FVector2D(MouseDelta.X - LastMousePosition.X, MouseDelta.Y - LastMousePosition.Y);
	if (MouseDelta.Size() != 0)
	{
		bEnableMouseOverEvents = true;
		bShowMouseCursor = true;
		GetWorldTimerManager().ClearTimer(MouseDeltaTimer);
	}
	else if(GetWorldTimerManager().GetTimerElapsed(MouseDeltaTimer) >= 1.0)
	{
		bEnableMouseOverEvents = false;
		bShowMouseCursor = false;
		GetWorldTimerManager().ClearTimer(MouseDeltaTimer);
	}
	else if(!GetWorldTimerManager().IsTimerActive(MouseDeltaTimer) && bEnableMouseOverEvents)
	{
		GetWorldTimerManager().SetTimer(MouseDeltaTimer, 2.0F, false);
	}
	
	GetMousePosition(LastMousePosition.X, LastMousePosition.Y);
}


