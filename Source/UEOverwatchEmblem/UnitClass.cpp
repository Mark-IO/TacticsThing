// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "WeaponRanks.h"
#include "UnitClass.h"

AUnitClass::AUnitClass()
{
	CostOfMove.Add(ETileTypeEnum::ET_Default, 1);
	CostOfMove.Add(ETileTypeEnum::ET_Impassable, -1);
	CostOfMove.Add(ETileTypeEnum::ET_HealthPack, 1);
	CostOfMove.Add(ETileTypeEnum::ET_Chasm, -1);
	CostOfMove.Add(ETileTypeEnum::ET_Woods, 2);
	CostOfMove.Add(ETileTypeEnum::ET_Water, 5);
	CostOfMove.Add(ETileTypeEnum::ET_SmallWall, -1);
	Affinities.Add(EAffinityEnum::EA_None);
	Name = "None";
	Description = "Add a description";
}

