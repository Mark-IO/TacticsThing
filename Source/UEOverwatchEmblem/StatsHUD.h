// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/HUD.h"
#include "MyTile.h"
#include "StatsHUD.generated.h"

/**
 *	Things i want to implement:
	On map:

		- HP counter underneath character
		- Tile information under cursor

	When the cursor is over a character:
		- Show possible movement range with minimum number of moves to reach said square

	On combat preview:
		- Relevant stat between defense and resistance, depending on the opponent's weapon
		- How much damage will be done, hit, crit, dodge, avoid, and skills.
 */
UCLASS()
class UEOVERWATCHEMBLEM_API AStatsHUD : public AHUD
{
	GENERATED_BODY()

protected:
	/** draw health bars for actors */
	void DrawActorsHealth();

	void DrawTileInfo(AMyTile* cursorTile);

	
public:
	/** current UI scale */
	float UIScale;

	AStatsHUD(const FObjectInitializer& ObjectInitializer);
	void DrawHUD() override;

	
	

	
};
