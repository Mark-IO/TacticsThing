// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "PlayerCursor.h"
#include "CursorActor.h"
#include "MapGameStateBase.h"
#include "UnitPawn.h"
#include "MyTile.h"


FTimerHandle AMyTile::InputDelayTimer = FTimerHandle();
int32 AMyTile::tileSize = 0;

AMyTile::AMyTile(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	//Allows for the character to have a system which can update every frame
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
	PrimaryActorTick.bAllowTickOnDedicatedServer = false;

	//Sets an invisble trigger that the mouse can detect
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BoxComponent"));
	RootComponent = MeshComponent;
	MeshComponent->OnBeginCursorOver.AddDynamic(this, &AMyTile::OnBeginCursor);
	MeshComponent->OnEndCursorOver.AddDynamic(this, &AMyTile::OnEndCursor);
	SetActorHiddenInGame(true);
	
}
void AMyTile::BeginPlay()
{
	
	Super::BeginPlay();
	P1Controller = (GetWorld()->GetFirstPlayerController());
	CursorPawn = P1Controller->AcknowledgedPawn;
	if (tileSize != 100 * GetActorScale().X && tileSize != 0)
	{
		//Send a warning to indicate that some of the tiles are not uniformly scaled
		UE_LOG(LogTemp, Warning, TEXT("Tile %s not uniformly scaled"), *GetName());
	}
	tileSize = 100 * GetActorScale().X;
}

void AMyTile::OnBeginCursor(class UPrimitiveComponent* OtherComponent)
{
	/*
		This will move the cursor to where the tile the mouse is over within the tick method.
	*/
	AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());
	if (state->GameplayState == EGameplayStateEnum::EG_BaseMenu || state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
	{
		bIsCursorTarget = true;
	}
	
}

void AMyTile::OnEndCursor(class UPrimitiveComponent* OtherComponent)
{
	bIsCursorTarget = false;
}

bool AMyTile::IsTileAdjacentToPath(AMyTile* tile)
{
	if (AUnitPawn::CurrentPath.Num() == 0)
	{
		return false;
	}
	AMyTile* lastTile = AUnitPawn::CurrentPath.Last();
	for (int j = 0; j < 4; j++)
	{

		FVector Start;
		if (j == 0)
		{
			Start = lastTile->GetActorLocation() + FVector(lastTile->tileSize, 0, 0);
		}
		else if (j == 1)
		{
			Start = lastTile->GetActorLocation() + FVector(-lastTile->tileSize, 0, 0);
		}
		else if (j == 2)
		{
			Start = lastTile->GetActorLocation() + FVector(0, lastTile->tileSize, 0);
		}
		else if (j == 3)
		{
			Start = lastTile->GetActorLocation() + FVector(0, -lastTile->tileSize, 0);
		}

		if (tile->GetActorLocation().X == Start.X && tile->GetActorLocation().Y == Start.Y)
		{

			return true;
		}
	}
	return false;
}

void AMyTile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsCursorTarget && !GetWorldTimerManager().IsTimerActive(InputDelayTimer))
	{
		Cast<ACursorActor>(CursorPawn)->CurrentTile = this;
		GetWorldTimerManager().SetTimer(InputDelayTimer, 0.15, false);
		bIsCursorTarget = false;
		AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());
		if ( state->GameplayState == EGameplayStateEnum::EG_MoveMenu ) 
		{
			if (AUnitPawn::CurrentPath.Num() == 0)
			{
				/*
				Check to see if this tile is one square away from the starting position
					Add to the current path if it is, or set it to the shortest path if it isn't
				*/
				for (int i = 0; i < AUnitPawn::SelectedUnitPawn->moveRange.Num(); i++)
				{
					if (this == AUnitPawn::SelectedUnitPawn->moveRange[i].GetTile())
					{
						if (AUnitPawn::SelectedUnitPawn->moveRange[i].GetShortestPath().Num() == 1)
						{
							AUnitPawn::CurrentPath.Add(Cast<ACursorActor>(CursorPawn)->CurrentTile);
						}
						else
						{
							AUnitPawn::CurrentPath = AUnitPawn::SelectedUnitPawn->moveRange[i].GetShortestPath();
						}
						break;
					}
				}
				
			}
			else if ( AUnitPawn::CurrentPath.Num() == 1 && Cast<ACursorActor>(CursorPawn)->CurrentTile == AUnitPawn::SelectedUnitPawn->currentLocation
				|| AUnitPawn::CurrentPath.Num() > 1 && Cast<ACursorActor>(CursorPawn)->CurrentTile == AUnitPawn::CurrentPath.Last(1))
			{
				AUnitPawn::CurrentPath.Pop();
				
			}
			else
			{
				//Check if tile is within any possible movement range
				for (int i = 0; i < AUnitPawn::SelectedUnitPawn->moveRange.Num(); i++)
				{
					if (Cast<ACursorActor>(CursorPawn)->CurrentTile == AUnitPawn::SelectedUnitPawn->moveRange[i].GetTile())
					{

						//Find out if the tile the cursorPawn is over is one of the tiles in the one of four adjacent spaces of the lastTile in the currentpath
						bool isAdjacent = IsTileAdjacentToPath(this);
						
            
                        if (isAdjacent)
						{
                            int32 moveCost = *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(this->TileTypeEnum);
                            bool bCanPass = true;

                            if (this->bIsOccupied)
                            {
                                if (AUnitPawn::GetUnitPawnAt(this)->Faction == EFactionEnum::EF_Enemy && AUnitPawn::SelectedUnitPawn->Faction != EFactionEnum::EF_Enemy
                                    || AUnitPawn::GetUnitPawnAt(this)->Faction != EFactionEnum::EF_Enemy && AUnitPawn::SelectedUnitPawn->Faction == EFactionEnum::EF_Enemy)
                                {
                                    bCanPass = false;
                                }
                            }
							if (this->TileTypeEnum == ETileTypeEnum::ET_Impassable)
							{
								bCanPass = false;
							}

							/*
							if (this->TileTypeEnum == ETileTypeEnum::ET_Chasm && !AUnitPawn::GetUnitPawnAt(this)->CurrentClass->bCanFly)
							{
								bCanPass = false;
							}
							*/

                            /*
                            moveCost < 0 means that the class can't access the tile
                            moveCost = 0 means that the tile type isn't accounted for in unit class
                            */
                            if (bCanPass && moveCost > 0)
                            {
                                int32 sumCost = 0;
                                //Check and see if the total cost is greater than available move
                                for (int k = 0; k < AUnitPawn::CurrentPath.Num(); k++)
                                {
                                    sumCost += *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(AUnitPawn::CurrentPath[k]->TileTypeEnum);
                                }
                                sumCost += moveCost;
                                if (sumCost <= AUnitPawn::SelectedUnitPawn->GetTotalMovement())
                                {
                                    AUnitPawn::CurrentPath.Add(this);
                                        
                                }
                                else
                                {
									if (this != AUnitPawn::SelectedUnitPawn->currentLocation)
									{
										TArray<AMyTile*> newPath = AUnitPawn::SelectedUnitPawn->moveRange[i].GetShortestPath();
										if (newPath.Num() > 0)
										{
											newPath.RemoveAt(0); //We don't need the base tile in the current path
										}
										AUnitPawn::CurrentPath = newPath;
										AUnitPawn::CurrentPath.Add(this);
									}
									else
									{
										AUnitPawn::CurrentPath.Empty();
									}
                                }
                            }
                                        
                            break;
                            
                        }
								
						else
						{
							if (this != AUnitPawn::SelectedUnitPawn->currentLocation)
							{
								AUnitPawn::CurrentPath.Empty();
								TArray<AMyTile*> newPath = AUnitPawn::SelectedUnitPawn->moveRange[i].GetShortestPath();
								if (newPath.Num() > 0)
								{
									newPath.RemoveAt(0); //We don't need the base tile in the current path
								}

								AUnitPawn::CurrentPath = newPath;
								AUnitPawn::CurrentPath.Add((Cast<ACursorActor>(CursorPawn)->CurrentTile));
							}
							else
							{
								AUnitPawn::CurrentPath.Empty();
							}
						}
					}
				}
			}
		}
	}
}

