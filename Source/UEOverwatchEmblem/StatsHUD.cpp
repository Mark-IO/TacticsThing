// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "CursorActor.h"
#include "MyTile.h"
#include "UnitPawn.h"
#include "MapGameStateBase.h"
#include "StatsHUD.h"


AStatsHUD::AStatsHUD(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	UIScale = 1;

}

void AStatsHUD::DrawActorsHealth()
{

}

void AStatsHUD::DrawTileInfo(AMyTile* cursorTile)
{
	FString defenseString = FString::FromInt((cursorTile->defenseBonus));
	FString avoidString = FString::FromInt((cursorTile->avoidBonus));
	FString healingString = FString::FromInt((cursorTile->healingBonus));

	DrawText(cursorTile->tileName, FColor(120, 68, 33), 0, 0, (UFont*)0, UIScale);
	DrawText(defenseString, FColor(255, 204, 170), 10, 30, (UFont*)0, UIScale);
	DrawText(avoidString, FColor(255, 204, 170), 30, 30, (UFont*)0, UIScale);
	DrawText(healingString, FColor(255, 204, 170), 50, 30, (UFont*)0, UIScale);
}

void AStatsHUD::DrawHUD()
{

	if (GEngine && GEngine->GameViewport)
	{
		FVector2D ViewportSize;
		GEngine->GameViewport->GetViewportSize(ViewportSize);
		UIScale = ViewportSize.X / 1024.0F;
	}


	Super::DrawHUD();

	ACursorActor* cursor = Cast<ACursorActor>(GetOwningPawn());
	AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());

	DrawTileInfo(cursor->CurrentTile);
	if (state->GameplayState == EGameplayStateEnum::EG_BaseMenu)
	{
		if (cursor->CurrentTile->bIsOccupied)
		{

			AUnitPawn* unit = AUnitPawn::GetUnitPawnAt(cursor->CurrentTile);
			for (int i = 0; i < unit->moveRange.Num(); i++)
			{
				FVector projection = Project(unit->moveRange[i].GetTile()->GetActorLocation());
				FString numberString = FString::FromInt((unit->moveRange[i].GetMinCost()));
				if (numberString != "0")
				{
					float textWidth = 0;
					float textHeight = 0;
					GetTextSize(numberString, textWidth, textHeight, (UFont*)0, UIScale);

					DrawText(numberString, FColor::White, projection.X - textWidth / 2, projection.Y - textHeight / 2, (UFont*)0, UIScale);
				}
			}
		}
	}
	if (state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
	{
		int32 sumCost = 0;

		for (int i = 0; i < AUnitPawn::CurrentPath.Num(); i++)
		{
			sumCost += *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(AUnitPawn::CurrentPath[i]->TileTypeEnum);
			FVector projection = Project(AUnitPawn::CurrentPath[i]->GetActorLocation());

			float textWidth = 0;
			float textHeight = 0;
			GetTextSize(FString::FromInt(sumCost), textWidth, textHeight, (UFont*)0, UIScale);
			DrawText(FString::FromInt(sumCost), FColor::White, projection.X - textWidth / 2, projection.Y - textHeight / 2, (UFont*)0, UIScale);
		}
	}
	
}

