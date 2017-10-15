// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "MapGameStateBase.h"


AMapGameStateBase::AMapGameStateBase(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	GameplayState = EGameplayStateEnum::EG_BaseMenu;
	turnCount = 1;
}

