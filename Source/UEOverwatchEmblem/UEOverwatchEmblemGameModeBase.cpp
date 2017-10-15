// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "UEOverwatchEmblemGameModeBase.h"
#include "PlayerCursor.h"
#include "StatsHUD.h"
#include "MapGameStateBase.h"
#include "CursorActor.h"


AUEOverwatchEmblemGameModeBase::AUEOverwatchEmblemGameModeBase()
{
	this->PlayerControllerClass = APlayerCursor::StaticClass();
	this->DefaultPawnClass = nullptr;
	this->HUDClass = AStatsHUD::StaticClass();
	this->GameStateClass = AMapGameStateBase::StaticClass();
}

