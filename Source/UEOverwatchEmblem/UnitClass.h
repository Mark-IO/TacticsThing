#include "Item.h"
#include "GameFramework/Actor.h"
#include "MyTile.h"
#include "WeaponRanks.h"
#include "UnitClass.generated.h"

#pragma once

/**
 *	-classes have a name
	-description
	-GrowthRatebonuses
	-Stat Bonuses
	-StatCaps
	-Movement cost per tile type
	- Weapon types used & max rank

	-Generic male and female models & sprites
 */
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EWeaponEnum : uint8
{
	ET_Staff			UMETA(DisplayName = "Staff"),
	ET_Sword			UMETA(DisplayName = "Sword"),
	ET_Lance			UMETA(DisplayName = "Lance"),
	ET_Axe				UMETA(DisplayName = "Axe"),
	ET_Tome				UMETA(DisplayName = "Tome"),
	ET_Dagger			UMETA(DisplayName = "Dagger"),
	ET_Bow				UMETA(DisplayName = "Bow"),
	ET_Gun				UMETA(DisplayName = "Gun"),
	ET_Breath			UMETA(DisplayName = "Breath"),
	ET_Empty			UMETA(DisplayName = "Empty")
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EAffinityEnum : uint8
{
	EA_None				UMETA(DisplayName = "None"),
	EA_Horse			UMETA(DisplayName = "Horse"),
	EA_Armor			UMETA(DisplayName = "Armor"),
	EA_Flying			UMETA(DisplayName = "Flying"),
	EA_Dragon			UMETA(DisplayName = "Dragon")

};

UCLASS()
class UEOVERWATCHEMBLEM_API AUnitClass : public AActor
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TArray<EAffinityEnum> Affinities;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TMap<EWeaponEnum, int32> MaxWeaponRanks;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TMap<ETileTypeEnum, int32> CostOfMove;	//Let values of -1 be uncrossable
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 MoveBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 HealthBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 HealthGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 HealthCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 StrengthBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 StrengthGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 StrengthCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 MagicBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 MagicGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 MagicCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 SkillBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 SkillGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 SkillCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 SpeedBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 SpeedGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 SpeedCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 LuckBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 LuckGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 LuckCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 DefenseBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 DefenseGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 DefenseCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 ResistanceBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Growths)
		int32 ResistanceGrowth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Caps)
		int32 ResistanceCap;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Description)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Description)
		FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 HitBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 CritBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 AvoidBonus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ClassBonus)
		int32 DodgeBonus;

	AUnitClass();
};
