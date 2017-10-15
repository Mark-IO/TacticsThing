// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "PaperSpriteComponent.h"
#include "PaperFlipbookComponent.h"
#include "MapGameStateBase.h"
#include "CursorActor.h"
#include "UnitPawn.h"

AUnitPawn* AUnitPawn::SelectedUnitPawn = nullptr;
TArray<AMyTile*> AUnitPawn::CurrentPath;

// Sets default values
AUnitPawn::AUnitPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<UBoxComponent>(TEXT("Origin"));
	RootComponent = Origin;

	mapSprite = CreateDefaultSubobject<UPaperFlipbookComponent>(TEXT("MapFlipBook"));
	mapSprite->SetupAttachment(RootComponent);
	mapSprite->SetRelativeLocation(FVector::ZeroVector);

	portraitSprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("PortraitSprite"));
	portraitSprite->SetupAttachment(RootComponent);
	portraitSprite->SetRelativeLocation(FVector::ZeroVector);

	characterModel = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterModel"));
	characterModel->SetupAttachment(RootComponent);
	characterModel->SetRelativeLocation(FVector::ZeroVector);

	items.SetNum(5, false);
	moveCount = 0;
}

int32 AUnitPawn::GetTotalHealth()
{
	return totalHealth;
}

int32 AUnitPawn::GetTotalStrength()
{
	return totalStrength;
}

int32 AUnitPawn::GetTotalMagic()
{
	return totalMagic;
}

int32 AUnitPawn::GetTotalSkill()
{
	return totalSkill;
}

int32 AUnitPawn::GetTotalSpeed()
{
	return totalSpeed;
}

int32 AUnitPawn::GetTotalLuck()
{
	return totalLuck;
}

int32 AUnitPawn::GetTotalDefense()
{
	return totalDefense;
}

int32 AUnitPawn::GetTotalResistance()
{
	return totalResistance;
}

int32 AUnitPawn::GetTotalMovement()
{
	return totalMovement;
}

int8 AUnitPawn::GetAttack()
{
	return attack;
}

int8 AUnitPawn::GetHit()
{
	return hit;
}

int8 AUnitPawn::GetCrit()
{
	return crit;
}

int8 AUnitPawn::GetAvoid()
{
	return avoid;
}

int8 AUnitPawn::GetDodge()
{
	return dodge;
}

bool AUnitPawn::HasMovedThisTurn()
{
	return bHasMovedThisTurn;
}

AUnitPawn* AUnitPawn::GetUnitPawnAt(AMyTile* tile)
{
	if (tile->bIsOccupied)
	{
		FVector Start = tile->GetActorLocation() + FVector(0, 0, 50);
		FVector End = Start + FVector(0, 0, -100);
		TArray<FHitResult> HitData;

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		tile->GetWorld()->LineTraceMultiByChannel(
			HitData,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		for (int32 b = 0; b < HitData.Num(); b++)
		{
			if (HitData[b].GetActor()->IsA(AUnitPawn::StaticClass()))
			{
				return Cast<AUnitPawn>(HitData[b].GetActor());
				break;
			}

		}
		return nullptr;
	}
	return nullptr;	
}

void AUnitPawn::BuildActionList()
{
	bool canUseWeapon = false;
	bool hasItem = false;
	// delete the previous list as some actions are location dependant
	AUnitPawn::SelectedUnitPawn->ListOfActions.Empty();

	//Run through the selected character's inventory, any items in their inventory adds the "item" command
	//Add attack if any weapon the character has in their inventory can reach an enemy

	for (int i = 0; i < AUnitPawn::SelectedUnitPawn->items.Num(); i++)
	{
		if (AUnitPawn::SelectedUnitPawn->items[i] != NULL && AUnitPawn::SelectedUnitPawn->items[i]->TypeEnum != ETypeEnum::ET_Empty)
		{
			hasItem = true; 
			//AUnitPawn::SelectedUnitPawn->ListOfActions.AddUnique("Items");
		}
	}

	//Check if any allies are adjacent to the selected character, as that enables trading/shoving if that's available


	if (hasItem)
	{
		AUnitPawn::SelectedUnitPawn->ListOfActions.Add("Item");
	}
	//The player can always wait at a location

	AUnitPawn::SelectedUnitPawn->ListOfActions.Add("Wait");


}

void AUnitPawn::ResetMoveCount()
{
	moveCount = 0;
}

// Called when the game starts or when spawned
void AUnitPawn::BeginPlay()
{
	Super::BeginPlay();
	if (currentLocation == nullptr)
	{
		const FVector Start = GetActorLocation() + FVector(0, 0, 50);
		const FVector End = Start + FVector(0, 0, -100);
		TArray<FHitResult> HitData;

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->LineTraceMultiByChannel(
			HitData,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		for (int32 b = 0; b < HitData.Num(); b++)
		{
			if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
			{
				currentLocation = Cast<AMyTile>(HitData[b].GetActor());
				currentLocation->bIsOccupied = true;
			}

		}
	}
	else
	{
		currentLocation->bIsOccupied = true;
	}
	if ( CurrentClass )
	{
		CalcStats();
		CalcMovement();
	}
}

// Called every frame
void AUnitPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	ACursorActor* cursor = Cast<ACursorActor>(GetWorld()->GetFirstPlayerController()->GetPawn());
	AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());

	if (cursor->CurrentTile == currentLocation)
	{
		if (!bHasCursorEnteredThisTile)
		{
			CalcMovement();
			bHasCursorEnteredThisTile = true;
		}	 
	}
	else
	{
		bHasCursorEnteredThisTile = false;
	}

	if (AUnitPawn::SelectedUnitPawn == this && state && state->GameplayState == EGameplayStateEnum::EG_ActionMenu)
	{
		if (moveCount < AUnitPawn::CurrentPath.Num())
		{
			isAtDestination = false;
			FVector2D Location2D = FVector2D(AUnitPawn::SelectedUnitPawn->GetActorLocation());
			FVector2D Destination = FMath::Vector2DInterpConstantTo(Location2D, FVector2D(AUnitPawn::CurrentPath[moveCount]->GetActorLocation()), DeltaTime, 1000);
			AUnitPawn::SelectedUnitPawn->SetActorLocation(FVector(Destination, this->GetActorLocation().Z));
			if ((Destination - Location2D).IsNearlyZero())
			{
				moveCount++;
			}
		}
		else
		{
			isAtDestination = true;//Give the signal to summon the menu and enable controls
		}
	}
}

void AUnitPawn::AddBuff(FString Name, EStatTypeEnum Stat, int32 value)
{
	bool bAdded = false;
	for (int i = 0; i < CurrentBuffs.Num(); i++)
	{
		FString curName = CurrentBuffs[i].GetName();
		if (Name < curName)
		{
			continue;
		}
		else if (Name > curName)
		{
			CurrentBuffs.Insert(FBuffStruct(Name, Stat, value), i);
			bAdded = true;
			break;
		}
		else
		{
			if (Stat == CurrentBuffs[i].GetStat())
			{
				if (FMath::Abs(value) > FMath::Abs(CurrentBuffs[i].GetValue()))
				{
					CurrentBuffs[i].SetValue(value);
				}
				bAdded = true;
				break;
			}
			else
			{
				CurrentBuffs.Insert(FBuffStruct(Name, Stat, value), i + 1);
				bAdded = true;
				break;
			}
		}
	}
	if (!bAdded)
	{
		CurrentBuffs.Add(FBuffStruct(Name, Stat, value));
	}
	CalcStats();
}

void AUnitPawn::AddBuffs(FString Name, TArray<EStatTypeEnum> Stats, TArray<int32> values)
{
	bool bAdded = false;
	for (int i = 0; i < CurrentBuffs.Num(); i++)
	{
		FString curName = CurrentBuffs[i].GetName();
		if (Name < curName)
		{
			continue;
		}
		else if (Name > curName)
		{
			for (int j = 0; j < Stats.Num(); j++)
			{
				CurrentBuffs.Insert(FBuffStruct(Name, Stats[j], values[j]), i);
			}
			
			bAdded = true;
			break;
		}
		else
		{
			for (int j = 0; j < Stats.Num(); j++)
			{
				if (Stats[j] == CurrentBuffs[i].GetStat())
				{
					if (FMath::Abs(values[j]) > FMath::Abs(CurrentBuffs[i].GetValue()))
					{
						CurrentBuffs[i].SetValue(values[j]);
					}
					bAdded = true;
				}
				else
				{
					CurrentBuffs.Insert(FBuffStruct(Name, Stats[j], values[j]), i + 1);
					bAdded = true;
				}
			}
		}
	}
	if (!bAdded)
	{
		for (int j = 0; j < Stats.Num(); j++)
		{
			CurrentBuffs.Add(FBuffStruct(Name, Stats[j], values[j]));
		}
	}
	CalcStats();
}


void AUnitPawn::CalcStats()
{
	// -1 Calc base stats by combining class and unit stats together, applying caps as necessary
	// 
	// -2 Go trough the list of buffs/debuffs and apply them to the character.
	// -3 Derive a base statline for attack, hit, crit, and dodge from the weapon
	
	totalHealth = ((maxHealth + CurrentClass->HealthBonus > CurrentClass->HealthCap ) ? CurrentClass->HealthCap : maxHealth + CurrentClass->HealthBonus);
	totalMovement = baseMove + CurrentClass->MoveBonus;
	totalStrength = ((baseStrength + CurrentClass->StrengthBonus > CurrentClass->StrengthCap + capStrength) ? CurrentClass->StrengthCap + capStrength : baseStrength + CurrentClass->StrengthBonus);
	totalMagic = ((baseMagic + CurrentClass->MagicBonus > CurrentClass->MagicCap + capMagic) ? CurrentClass->MagicCap + capMagic : baseMagic + CurrentClass->MagicBonus);
	totalSkill = ((baseSkill + CurrentClass->SkillBonus > CurrentClass->SkillCap + capSkill) ? CurrentClass->SkillCap + capSkill : baseSkill + CurrentClass->SkillBonus);
	totalSpeed = ((baseSpeed + CurrentClass->SpeedBonus > CurrentClass->SpeedCap + capSpeed) ? CurrentClass->SpeedCap + capSpeed : baseSpeed + CurrentClass->SpeedBonus);
	totalLuck = ((baseLuck + CurrentClass->LuckBonus > CurrentClass->LuckCap + capLuck) ? CurrentClass->LuckCap + capLuck : baseLuck + CurrentClass->LuckBonus);
	totalDefense = ((baseDefense + CurrentClass->DefenseBonus > CurrentClass->DefenseCap + capDefense) ? CurrentClass->DefenseCap + capDefense : baseDefense + CurrentClass->DefenseBonus);
	totalResistance = ((baseResistance + CurrentClass->ResistanceBonus > CurrentClass->ResistanceCap + capResistance) ? CurrentClass->ResistanceCap + capResistance : baseResistance + CurrentClass->ResistanceBonus);

	for (int i = 0; i < CurrentBuffs.Num(); i++)
	{
		switch (CurrentBuffs[i].GetStat())
		{
			case EStatTypeEnum::ES_Health:
				totalHealth += CurrentBuffs[i].GetValue();
				if (currentHealth > totalHealth)
				{
					currentHealth = totalHealth;
				}
				break;

			case EStatTypeEnum::ES_Movement:
				totalMovement += CurrentBuffs[i].GetValue();
				if (totalMovement < 0)
				{
					totalMovement = 0;
				}
				break;

			case EStatTypeEnum::ES_Strength:
				totalStrength += CurrentBuffs[i].GetValue();
				break;

			case EStatTypeEnum::ES_Magic:
				totalMagic += CurrentBuffs[i].GetValue();
				break;

			case EStatTypeEnum::ES_Skill:
				totalSkill += CurrentBuffs[i].GetValue();
				break;

			case EStatTypeEnum::ES_Speed:
				totalSpeed += CurrentBuffs[i].GetValue();
				break;

			case EStatTypeEnum::ES_Luck:
				totalLuck += CurrentBuffs[i].GetValue();
				break;

			case EStatTypeEnum::ES_Defense:
				totalDefense += CurrentBuffs[i].GetValue();
				break;

			case EStatTypeEnum::ES_Resistance:
				totalResistance += CurrentBuffs[i].GetValue();
				break;
		}
	}
	if (equippedWeapon != nullptr)
	{
		/*
		if(equippedWeapon->TypeEnum == ETypeEnum::ET_Staff)
		{
			hit = ((totalMagic) + totalSkill +  ( CurrentClass->HitBonus) + (WeaponRankHitBonus) + staffInUse->Hit);
			attack = 0; 
			crit = 0;
		}
		else
		*/
		
		attack = (equippedWeapon->bUseStrengthOrMagic ? totalStrength : totalMagic) + equippedWeapon->might /* + WeaponRankAtkBonus*/;
		
		hit = (int32)((totalSkill * 1.5) + (totalLuck * .5) + ( CurrentClass->HitBonus)  /*+ (WeaponRankHitBonus)*/ + (equippedWeapon->hit));
		crit = ((int32)(totalSkill - 4) / 2) + equippedWeapon->crit + CurrentClass->CritBonus;
		avoid = (int)((totalSpeed * 1.5) + (totalLuck * 0.5) + equippedWeapon->avoid + CurrentClass->AvoidBonus);
		dodge = (int)((totalLuck / 2) + equippedWeapon->dodge + CurrentClass->DodgeBonus);
	}
	else
	{
		attack = 0;
		hit = 0;
		crit = 0;
		avoid = (int)((totalSpeed * 1.5) + (totalLuck * 0.5) + CurrentClass->AvoidBonus);
		dodge = (int)((totalLuck / 2) + CurrentClass->DodgeBonus);
	}
	
	CalcMovement();
}

void AUnitPawn::CalcMovement()
{
	moveRange.Empty();
	int32 moveLeft = totalMovement;
	
	if (moveRange.Num() == 0)
	{
		moveRange.Add(FMoveRangeStruct(currentLocation, 0)); //The first element is always the root
	}
	TArray<AMyTile*> ProcessPath;

	ProcessPath.Add(currentLocation); //Begin with root

	while (ProcessPath.Num() != 0)
	{
		AMyTile* CurTile = ProcessPath[0];
		TArray<AMyTile*> tilesToAddToQueue;
		AMyTile* foundTile;
		ProcessPath.RemoveAt(0);
		for (int i = 0; i < moveRange.Num(); i++)
		{
			if (moveRange[i].GetTile() == CurTile)
			{
				moveLeft = totalMovement - moveRange[i].GetMinCost(); 
			}
		}
		
		//Check tile in +X direction (above)

		FVector Start = CurTile->GetActorLocation() + FVector(currentLocation->tileSize, 0, 50);
		FVector End = Start + FVector(0, 0, -100);
		TArray<FHitResult> HitData;

		FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
		RV_TraceParams.bTraceComplex = true;
		RV_TraceParams.bTraceAsyncScene = true;
		RV_TraceParams.bReturnPhysicalMaterial = false;

		GetWorld()->LineTraceMultiByChannel(
			HitData,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		for (int32 b = 0; b < HitData.Num(); b++)
		{
			if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
			{
				foundTile = Cast<AMyTile>(HitData[b].GetActor());
				int32 moveCost = *CurrentClass->CostOfMove.Find(foundTile->TileTypeEnum);
				bool bCanPass = true;
				
				if (foundTile->bIsOccupied)
				{
					if (GetUnitPawnAt(foundTile)->Faction == EFactionEnum::EF_Enemy && Faction != EFactionEnum::EF_Enemy
						|| GetUnitPawnAt(foundTile)->Faction != EFactionEnum::EF_Enemy && Faction == EFactionEnum::EF_Enemy)
					{
						bCanPass = false;
					}
				}
				/*
				moveCost < 0 means that the class can't access the tile
				moveCost = 0 means that the tile type isn't accounted for in unit class
				*/
				if ( bCanPass && moveCost > 0 && moveLeft >= moveCost)
				{
					bool bIsTileInArray = false;
					TArray<AMyTile*> subPathTile;
					//Check the moveRange array to see if it has the current tile or not
					for (int i = 0; i < moveRange.Num(); i++)
					{
						AMyTile* comparedTile = moveRange[i].GetTile();
						if (CurTile == comparedTile)
						{
							subPathTile = moveRange[i].GetShortestPath();
						}
						if (foundTile == comparedTile)
						{
							//Compare the two and record the shortest path with min movement
							if (totalMovement - moveLeft + moveCost < moveRange[i].GetMinCost())
							{
								//Success! add into the queue later, update shortest path and mincost
								tilesToAddToQueue.Add(foundTile);
								moveRange[i].SetMinCost(totalMovement - moveLeft + moveCost);

								subPathTile.Add(CurTile);
								moveRange[i].SetShortestPath(subPathTile);
								
							}
							bIsTileInArray = true;
							break;
						}
					}
					if (!bIsTileInArray)
					{
						tilesToAddToQueue.Add(foundTile);
						subPathTile.Add(CurTile);
						moveRange.Add(FMoveRangeStruct(foundTile, (totalMovement - moveLeft + moveCost), subPathTile));
					}
				}
			}

		}

		//Check tile in -X direction (below)

		Start = CurTile->GetActorLocation() + FVector(-currentLocation->tileSize, 0, 50);
		End = Start + FVector(0, 0, -100);
		HitData.Empty();


		GetWorld()->LineTraceMultiByChannel(
			HitData,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		for (int32 b = 0; b < HitData.Num(); b++)
		{
			if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
			{
				foundTile = Cast<AMyTile>(HitData[b].GetActor());
				int32 moveCost = *CurrentClass->CostOfMove.Find(foundTile->TileTypeEnum);
				bool bCanPass = true;

				if (foundTile->bIsOccupied)
				{
					if (GetUnitPawnAt(foundTile)->Faction == EFactionEnum::EF_Enemy && Faction != EFactionEnum::EF_Enemy
						|| GetUnitPawnAt(foundTile)->Faction != EFactionEnum::EF_Enemy && Faction == EFactionEnum::EF_Enemy)
					{
						bCanPass = false;
					}
				}
				/*
				moveCost < 0 means that the class can't access the tile
				moveCost = 0 means that the tile type isn't accounted for in unit class
				*/
				if (bCanPass && moveCost > 0 && moveLeft >= moveCost)
				{
					bool bIsTileInArray = false;
					TArray<AMyTile*> subPathTile;
					//Check the moveRange array to see if it has the current tile or not
					for (int i = 0; i < moveRange.Num(); i++)
					{
						AMyTile* comparedTile = moveRange[i].GetTile();
						if (CurTile == comparedTile)
						{
							subPathTile = moveRange[i].GetShortestPath();
						}
						if (foundTile == comparedTile)
						{
							//Compare the two and record the shortest path with min movement
							if (totalMovement - moveLeft + moveCost < moveRange[i].GetMinCost())
							{
								//Success! add into the queue later, update shortest path and mincost
								tilesToAddToQueue.Add(foundTile);
								moveRange[i].SetMinCost(totalMovement - moveLeft + moveCost);

								subPathTile.Add(CurTile);
								moveRange[i].SetShortestPath(subPathTile);

							}
							bIsTileInArray = true;
							break;
						}
					}
					if (!bIsTileInArray)
					{
						tilesToAddToQueue.Add(foundTile);
						subPathTile.Add(CurTile);
						moveRange.Add(FMoveRangeStruct(foundTile, (totalMovement - moveLeft + moveCost), subPathTile));
					}
				}
			}

		}

		//Check tile in Y direction (Right)

		Start = CurTile->GetActorLocation() + FVector(0, currentLocation->tileSize, 50);
		End = Start + FVector(0, 0, -100);
		HitData.Empty();

		GetWorld()->LineTraceMultiByChannel(
			HitData,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		for (int32 b = 0; b < HitData.Num(); b++)
		{
			if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
			{
				foundTile = Cast<AMyTile>(HitData[b].GetActor());
				int32 moveCost = *CurrentClass->CostOfMove.Find(foundTile->TileTypeEnum);
				bool bCanPass = true;

				//Units cannot pass units of opposing factions
				if (foundTile->bIsOccupied)
				{
					if (GetUnitPawnAt(foundTile)->Faction == EFactionEnum::EF_Enemy && Faction != EFactionEnum::EF_Enemy
						|| GetUnitPawnAt(foundTile)->Faction != EFactionEnum::EF_Enemy && Faction == EFactionEnum::EF_Enemy)
					{
						bCanPass = false;
					}
				}
				/*
				moveCost < 0 means that the class can't access the tile
				moveCost = 0 means that the tile type isn't accounted for in unit class
				*/
				if (bCanPass && moveCost > 0 && moveLeft >= moveCost)
				{
					bool bIsTileInArray = false;
					TArray<AMyTile*> subPathTile;
					//Check the moveRange array to see if it has the current tile or not
					for (int i = 0; i < moveRange.Num(); i++)
					{
						AMyTile* comparedTile = moveRange[i].GetTile();
						if (CurTile == comparedTile)
						{
							subPathTile = moveRange[i].GetShortestPath();
						}
						if (foundTile == comparedTile)
						{
							//Compare the two and record the shortest path with min movement
							if (totalMovement - moveLeft + moveCost < moveRange[i].GetMinCost())
							{
								//Success! add into the queue later, update shortest path and mincost
								tilesToAddToQueue.Add(foundTile);
								moveRange[i].SetMinCost(totalMovement - moveLeft + moveCost);

								subPathTile.Add(CurTile);
								moveRange[i].SetShortestPath(subPathTile);

							}
							bIsTileInArray = true;
							break;
						}
					}
					if (!bIsTileInArray)
					{
						tilesToAddToQueue.Add(foundTile);
						subPathTile.Add(CurTile);
						moveRange.Add(FMoveRangeStruct(foundTile, (totalMovement - moveLeft + moveCost), subPathTile));
					}
				}
			}

		}

		//Check tile in -Y direction (Left)
		Start = CurTile->GetActorLocation() + FVector(0, -currentLocation->tileSize, 50);
		End = Start + FVector(0, 0, -100);
		HitData.Empty();

		GetWorld()->LineTraceMultiByChannel(
			HitData,        //result
			Start,    //start
			End, //end
			ECC_Pawn, //collision channel
			RV_TraceParams
		);

		for (int32 b = 0; b < HitData.Num(); b++)
		{
			if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
			{
				foundTile = Cast<AMyTile>(HitData[b].GetActor());
				int32 moveCost = *CurrentClass->CostOfMove.Find(foundTile->TileTypeEnum);
				bool bCanPass = true;

				if (foundTile->bIsOccupied)
				{
					if (GetUnitPawnAt(foundTile)->Faction == EFactionEnum::EF_Enemy && Faction != EFactionEnum::EF_Enemy
						|| GetUnitPawnAt(foundTile)->Faction != EFactionEnum::EF_Enemy && Faction == EFactionEnum::EF_Enemy)
					{
						bCanPass = false;
					}
				}
				/*
				moveCost < 0 means that the class can't access the tile
				moveCost = 0 means that the tile type isn't accounted for in unit class
				*/
				if (bCanPass && moveCost > 0 && moveLeft >= moveCost)
				{
					bool bIsTileInArray = false;
					TArray<AMyTile*> subPathTile;
					//Check the moveRange array to see if it has the current tile or not
					for (int i = 0; i < moveRange.Num(); i++)
					{
						AMyTile* comparedTile = moveRange[i].GetTile();
						if (CurTile == comparedTile)
						{
							subPathTile = moveRange[i].GetShortestPath();
						}
						if (foundTile == comparedTile)
						{
							//Compare the two and record the shortest path with min movement
							if (totalMovement - moveLeft + moveCost < moveRange[i].GetMinCost())
							{
								//Success! add into the queue later, update shortest path and mincost
								tilesToAddToQueue.Add(foundTile);
								moveRange[i].SetMinCost(totalMovement - moveLeft + moveCost);

								subPathTile.Add(CurTile);
								moveRange[i].SetShortestPath(subPathTile);

							}
							bIsTileInArray = true;
							break;
						}
					}
					if (!bIsTileInArray)
					{
						tilesToAddToQueue.Add(foundTile);
						subPathTile.Add(CurTile);
						moveRange.Add(FMoveRangeStruct(foundTile, (totalMovement - moveLeft + moveCost), subPathTile));
					}
				}
			}

		}

		//Finally add any new elements into the queue
		ProcessPath.Append(tilesToAddToQueue);
	}

}

// Called to bind functionality to input
void AUnitPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

