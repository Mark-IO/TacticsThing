// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameStateBase.h"
#include "MapGameStateBase.generated.h"

/**
	Handles:
 		Turn count
		Current and previous menu modes player is on 
		whose turn it is
		number allies left
		number enemies left
		
 */

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EGameplayStateEnum : uint8
{
	EG_PrepMenu			UMETA(DisplayName = "PrepMenu"),
	EG_ViewMap			UMETA(DisplayName = "ViewMap"),
	EG_SelectUnitsMenu	UMETA(DisplayName = "SelectUnitsMenu"),
	EG_BaseMenu			UMETA(DisplayName = "Base"),
	EG_TurnMenu			UMETA(DisplayName = "TurnMenu"),
	EG_MoveMenu			UMETA(DisplayName = "MoveMenu"),
	EG_ActionMenu		UMETA(DisplayName = "ActionMenu"),
	EG_TargetMenu		UMETA(DispalyName = "TargetMenu"),
	EG_AttackMenu		UMETA(DisplayName = "AttackMenu"),
	EG_ItemMenu			UMETA(DisplayName = "ItemMenu"),
	EG_TradeMenu		UMETA(DisplayName = "TradeMenu"),
	EG_EnemyTurn		UMETA(DisplayName = "EnemyTurn"),
	EG_AllyTurn			UMETA(DisplayName = "AllyTurn"),
	EG_Battle			UMETA(DisplayName = "Battle"),
	EG_BattleAnim		UMETA(DisplayName = "BattleAnim")
};
UCLASS()
class UEOVERWATCHEMBLEM_API AMapGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	EGameplayStateEnum GameplayState;
	TArray<EGameplayStateEnum> PreviousStates; //Stack of previous states that the menu can go back to

	uint8 turnCount;



	AMapGameStateBase(const class FObjectInitializer& ObjectInitializer);
	
};
