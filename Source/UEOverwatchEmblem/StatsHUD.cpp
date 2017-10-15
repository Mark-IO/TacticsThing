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
	if (state->GameplayState == EGameplayStateEnum::EG_ActionMenu && AUnitPawn::SelectedUnitPawn->isAtDestination )
	{
		//Get the size of the list since it determines the size of the rectangle

		FVector projection = Project(AUnitPawn::SelectedUnitPawn->GetActorLocation());

		float textWidth = 0;
		float textHeight = 0;
		

		DrawRect(FLinearColor::Red, projection.X + 100 * UIScale, projection.Y - 25 * UIScale * AUnitPawn::SelectedUnitPawn->ListOfActions.Num(), 100 * UIScale, 50 * UIScale * AUnitPawn::SelectedUnitPawn->ListOfActions.Num());
		for (int i = 0; i < AUnitPawn::SelectedUnitPawn->ListOfActions.Num(); i++)
		{
			//Draw the text on top of the rectangle/texture
			GetTextSize(AUnitPawn::SelectedUnitPawn->ListOfActions[i], textWidth, textHeight, (UFont*)0, 2 * UIScale);

			// ListOfActions.Num() determines the starting point of the first text. Then from there, each text entry is spaced a fixed space apart
			DrawText(AUnitPawn::SelectedUnitPawn->ListOfActions[i], FColor::White, projection.X + 150 * UIScale - (textWidth / 2), projection.Y - 25 * UIScale * AUnitPawn::SelectedUnitPawn->ListOfActions.Num() + 50 * i * UIScale - (textHeight/2) + 25 * UIScale, (UFont*)0, 2 * UIScale );

		}
	}
	
}

