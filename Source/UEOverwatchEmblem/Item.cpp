// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "PaperSpriteComponent.h"
#include "UnitPawn.h"
#include "Item.h"


// Sets default values
AItem::AItem()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;
	TypeEnum = ETypeEnum::ET_Empty;
	Name = "";
	Description = "";
	maxUses = -1;
	usesLeft = -1;
	requiredRank = 0;
	might = 0; 
	hit = 0;
	crit = 0;
	avoid = 0;
	dodge = 0;
	Slays.Add(EEffectiveEnum::EE_None);
	range.Add(0);
	bIsLineOfSight = false;
	bUseStrengthOrMagic = true; //Uses strength
	bHitDefOrRes = true;		//Hits defense

	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("Sprite"));
	Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
}

// Called when the game starts or when spawned
void AItem::BeginPlay()
{
	Super::BeginPlay();
	
}
void AItem::OnUse()
{
	//Increase the staff rank if possible
	/*
	if (TypeEnum == ETypeEnum::ET_Staff && unitPawn.FactionEnum == EFactionEnum::EF_Ally)
	{
		
	}
	*/
	if (usesLeft > 0)
	{
		usesLeft--;
		if (usesLeft == 0)
		{			
			this->Destroy();
		}
	}
}
// Called every frame
void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

