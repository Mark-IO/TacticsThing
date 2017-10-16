/**
*	Items have a type: consumable, equipment, or Staves
		- Both have a name and a description
		- All have a sprite associated with them
		- Both can have current uses and max uses (equipment might be infinite)
		- Staves have charges but also require min rank to equip like equipment does
	Consumables:
		- Its pretty clear that items needs to have a virtual void method OnUse() to describe custom interaction
		- Destroyed when uses reaches 0

	Equipment:
		- Weapon type
		- Uses Strength or Magic
		- Hits Defense or Resistance
		- min weapon rank to equip (int value)
		- Might - Can have special conditions which cause this to vary
		- Effective vs (Horses, armor, dragon, etc)
		- Hit
		- Crit
		- Avoid
		- Dodge
		- Possible ranges, could be multiple values
		- Possible buffs but that is applied to character via AddBuff() in UnitPawn


*/

#pragma once

#include "GameFramework/Actor.h"
#include "WeaponRanks.h"
#include "Item.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETypeEnum : uint8
{
	ET_Consumable		UMETA(DisplayName = "Consumable"),
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
enum class EEffectiveEnum : uint8
{
	EE_None				UMETA(DisplayName = "None"),
	EE_Horse			UMETA(DisplayName = "Horse"),
	EE_Armor			UMETA(DisplayName = "Armor"),
	EE_Flying			UMETA(DisplayName = "Flying"),
	EE_Dragon			UMETA(DisplayName = "Dragon")

};

UCLASS()
class UEOVERWATCHEMBLEM_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class UPaperSpriteComponent* Sprite;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 maxUses; //Let -1 be an infinite use item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		int32 usesLeft; //Let -1 be an infinite use item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		ETypeEnum TypeEnum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		int32 requiredRank; 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		int32 might;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		int32 hit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		int32 crit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		int32 avoid;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		int32 dodge;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		TArray<EEffectiveEnum> Slays; //probably just one but could be multiple
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		TArray<int32> range;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		bool bIsLineOfSight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		bool bUseStrengthOrMagic;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Equipment)
		bool bHitDefOrRes;


	// Sets default values for this actor's properties
	AItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void OnUse();		//Make sure when override to call the Super method LAST

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
