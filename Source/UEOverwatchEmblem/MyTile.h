// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "MyTile.generated.h"

/**
* - Tiles represent individual unit of movement and battle.
* - Tiles have a type which has effects on defense, avoid, and healing.
* - Tiles can have a cost to access which varies by class
and some units cannot access certain tiles at all.
* - Tiles have a size that is uniform across the entire map
* - At the beginning of a map, player units can be on several positions
but become normal tiles once the maps starts.
* - Some tiles have enemies spawning from them, but not if it is occupied
- Tiles have a name so that duplicate rulesets are still unique to the map

*/


//Tiles types used to determine special rules
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETileTypeEnum : uint8
{
	ET_Default			UMETA(DisplayName = "Default"),				//Default no special properties
	ET_Impassable 		UMETA(DisplayName = "Impassable"),			//Basically a wall, can't be passed by any unit
	ET_HighGround		UMETA(DisplayName = "High ground"),			/*Experimental - Grants bonus avoid if target isn't in high ground
																	Conisder making height its own variable like FFtactics*/
	ET_HealthPack		UMETA(DisplayName = "Health Pack"),			//Restores health upon passing, takes time to respawn which varies depending on size
	ET_Chasm 			UMETA(DisplayName = "Chasm"),				//Only flying units can cross this area
	ET_Woods			UMETA(DisplayName = "Woods"),				//Some mounted classes have higher movements costs to access this area
	ET_Water			UMETA(DisplayName = "Water"),				//Most classes can't stand on but few can
	ET_SmallWall		UMETA(DisplayName = "Small Wall"),			//Can be flown past but not occupied, maybe not needed with the isOccupied tag
	ET_Interactive		UMETA(DisplayName = "Interactive"),			//Can be interacted with by standing on it.
	ET_Fort				UMETA(DisplayName = "Fort"),				//Has a movement cost for nonflying classes.
	ET_Ballista			UMETA(DisplayName = "Ballista")				//Ballista's can't be occupied, but have different values to show attack and hitrate
};

UCLASS(config = Game)
class UEOVERWATCHEMBLEM_API AMyTile : public AActor
{
	GENERATED_BODY()
	
	

protected:
	
	APlayerController* P1Controller;
	APawn* CursorPawn;
	//Updates every frame, governs the rules for acceleration for the dive action
	virtual void Tick(float DeltaTime) override;
	
	

public:
	static int32 tileSize;
	static FTimerHandle InputDelayTimer;
	bool bIsCursorTarget;
	bool bIsOccupied;
	
	//Collision and Transform for the tile
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class UStaticMeshComponent* MeshComponent;
	//Representation of tile type
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		ETileTypeEnum TileTypeEnum;
	//Name given to tile to show on UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString tileName;

	//Bonuses to stats that the tile can have
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 defenseBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 avoidBonus;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 healingBonus;

	//Experimental
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 height;

	//Qualities for a ballista
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpecialProperties)
		int32 ballistaAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SpecialProperties)
		int32 ballistaHitRate;

	AMyTile(const FObjectInitializer& ObjectInitializer);

	void BeginPlay() override;
	static bool IsTileAdjacentToPath(AMyTile* tile);
	UFUNCTION()
		void OnBeginCursor(class UPrimitiveComponent* OtherComponent);
	UFUNCTION()
		void OnEndCursor(class UPrimitiveComponent* OtherComponent);
	
};