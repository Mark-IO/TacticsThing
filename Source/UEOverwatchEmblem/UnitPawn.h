// Fill out your copyright notice in the Description page of Project Settings.
/*
	Units have
	- a name
	- a name has lore
	- a current class
	- a list of possible classes
	- Current Level
	- Internal Level
	- Experience
	- Personal skill(s)
	- Equipped skills
	- Learned skills
	- Attack (Calculated)
	- Crit (Calculated)
	- Hit (Calcualted)
	- Avoid (Calculated)
	- Dodge (Calculated)

	- Base movement(boots used)
	- Personal proficiency by weapon type

	- Current Health
	- Max Health
	- Current Shield
	- Max Shield
	- Current Armor
	- Max Armor

	- Base Strength
	- Base Magic
	- Base Skill
	- Base Speed
	- Base Luck
	- Base Defense
	- Base Resistance

	- Growth Health
	- Growth Strength
	- Growth Magic
	- Growth Skill
	- Growth Speed
	- Growth Luck
	- Growth Defense
	- Growth Resistance

	- Cap Strength
	- Cap Magic
	- Cap Skill
	- Cap Speed
	- Cap Luck
	- Cap Defense
	- Cap Resistance

	- Storing buffs and debuffs applied to the correct stat type with an explanation why 
		buff/debuff is applied.

	- Ultimate type
	- Ultimate charge cap
	- Current Ultimate charge

	- Faction
	- Faction Color

	- Potrait Sprite
	- Map Sprite (probably just the face)
	- Character Model (probably just face but might just make it complete for special characters)

	- Inventories slots 1-5
	- Equipped weapon
	- Tile character is currently under
*/
#pragma once

#include "GameFramework/Pawn.h"
#include "UnitClass.h"
#include "Item.h"
#include "MyTile.h"
#include "UnitPawn.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EFactionEnum : uint8
{
	EF_Ally				UMETA(DisplayName = "Ally"),
	EF_Enemy			UMETA(DisplayName = "Enemy"),
	EF_Other			UMETA(DisplayName = "Other")	
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EStatTypeEnum : uint8
{
	ES_Health			UMETA(DisplayName = "Health"),
	ES_Movement			UMETA(DisplayName = "Movement"),
	ES_Strength			UMETA(DisplayName = "Strength"),
	ES_Magic			UMETA(DisplayName = "Magic"),
	ES_Skill			UMETA(DisplayName = "Skill"),
	ES_Speed			UMETA(DisplayName = "Speed"),
	ES_Luck				UMETA(DisplayName = "Luck"),
	ES_Defense			UMETA(DisplayName = "Defense"),
	ES_Resistance		UMETA(DisplayName = "Resistance"),
	ES_Hit				UMETA(DisplayName = "Hit"),
	ES_Avoid			UMETA(DisplayName = "Avoid"),
	ES_Crit				UMETA(DisplayName = "Crit"),
	ES_Dodge			UMETA(DisplayName = "Dodge")
};

USTRUCT() 
struct FBuffStruct 
{ 
	GENERATED_USTRUCT_BODY() 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff/Debuff")
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff/Debuff")
		EStatTypeEnum Stat;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Buff/Debuff")
		int32 Value;

	FBuffStruct()
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		Name = NULL;
		Stat = EStatTypeEnum::ES_Health;
		Value = 0;
	}
	FBuffStruct(FString newName, EStatTypeEnum newStat, int32 newValue)
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		Name = newName;
		Stat = newStat;
		Value = newValue;
	}
	
	FString GetName()
	{
		return Name;
	}
	EStatTypeEnum GetStat()
	{
		return Stat;
	}
	int32 GetValue()
	{
		return Value;
	}
	void SetValue(int32 newValue)
	{
		Value = newValue;
	}
	
};

USTRUCT()
struct FMoveRangeStruct
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
		AMyTile* tile;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Location")
		int32 minCost;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Location")
		TArray<AMyTile*> ShortestPath;	
	
	FMoveRangeStruct()
	{
		tile = NULL;
		minCost = 0;
	}
	FMoveRangeStruct(AMyTile* newTile, int32 newCost)
	{
		//ONLY to be used on the current tile
		tile = newTile;
		minCost = newCost;
	}
	FMoveRangeStruct(AMyTile* newTile, int32 newCost, TArray<AMyTile*>& newPath)
	{
		//Always initialize your USTRUCT variables!
		//   exception is if you know the variable type has its own default constructor
		tile = newTile;
		minCost = newCost;
		ShortestPath = newPath;
	}
	int32 GetMinCost()
	{
		return minCost;
	}
	void SetMinCost(int32 newCost)
	{
		minCost = newCost;
	}
	AMyTile* GetTile()
	{
		return tile;
	}
	void SetTile(AMyTile* newTile)
	{
		tile = newTile;
	}
	TArray<AMyTile*> GetShortestPath()
	{
		return ShortestPath;
	}

	void SetShortestPath(TArray<AMyTile*> newTile)
	{
		ShortestPath = newTile;
	}


};

UCLASS()
class UEOVERWATCHEMBLEM_API AUnitPawn : public APawn
{
	GENERATED_BODY()

public:
	static AUnitPawn* SelectedUnitPawn;
	static TArray<AMyTile*> CurrentPath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString nameDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		AUnitClass* CurrentClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TArray<AUnitClass*> PersonalClasses;


	/*Stats to keep track of leveling and experience*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		uint8 currentLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		uint8 internalLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		uint8 experience;

	//ASkill PersonalSkill;
	//TArray<ASkill> EquippedSkills;
	//TArray<ASKill> LearnedSkills;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		TArray<uint8> WeaponProf;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 currentHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 maxHealth;
	//Shields and armor are health subtypes that can either regenerate out of combat or apply extra defenses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 currentShield;	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 maxShield;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 currentArmor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 maxArmor;

	//Bases for the unit itself pre class bonuses
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseMove;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseLuck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bases)
		uint8 baseResistance;


	//Chances certain stat goes up on levelup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthLuck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		float growthResistance;

	//Modifiers to caps based on personal
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capSkill;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capLuck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capDefense;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		uint8 capResistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
		TArray<FBuffStruct> CurrentBuffs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		EFactionEnum Faction;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FColor FactionColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UBoxComponent* Origin;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSpriteComponent* portraitSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperFlipbookComponent* mapSprite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class USkeletalMeshComponent* characterModel;


	// Items
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
		TArray<AItem*> items;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
		AItem* equippedWeapon;

	/*Tile to represent where the unit currently is*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Location)
		AMyTile* currentLocation;
	/*Array of possible tiles unit can reach with minimum costs*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Location)
		TArray<FMoveRangeStruct> moveRange;


	// Sets default values for this pawn's properties
	AUnitPawn();
	int32 GetTotalHealth();
	int32 GetTotalStrength();
	int32 GetTotalMagic();
	int32 GetTotalSkill();
	int32 GetTotalSpeed();
	int32 GetTotalLuck();
	int32 GetTotalDefense();
	int32 GetTotalResistance();
	int32 GetTotalMovement();

	int8 GetAttack();
	int8 GetHit();
	int8 GetCrit();
	int8 GetAvoid();
	int8 GetDodge();
	bool HasMovedThisTurn();

protected:

	//Used to play the follow path animation
	int32 moveCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Debug)
		bool bHasCursorEnteredThisTile;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Debug)
		bool bHasMovedThisTurn;

	//Final value after buffs/debuffs, class bonuses, etc is applied.		
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalStrength;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalMagic;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalSkill;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalLuck;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalDefense;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalResistance;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 totalMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 attack;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 hit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 crit;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 avoid;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Stats)
		uint8 dodge;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void CalcMovement();
	void CalcStats();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//void AddBuff(FBuffStruct buff);
	void AddBuff(FString Name, EStatTypeEnum Stat, int32 value);
	void AddBuffs(FString Name, TArray<EStatTypeEnum> Stats, TArray<int32> values);	// Overload so that one source to reduce recalculations
	
	// Have a method to quickly get a unitpawn at the specified tile
	static AUnitPawn* GetUnitPawnAt(AMyTile* tile);

	// Reset for when the player can travel a new path.
	void ResetMoveCount();
	
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
