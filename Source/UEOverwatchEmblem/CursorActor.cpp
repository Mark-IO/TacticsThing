// Fill out your copyright notice in the Description page of Project Settings.

#include "UEOverwatchEmblem.h"
#include "MyTile.h"
#include "MapGameStateBase.h"
#include "PaperSpriteComponent.h"
#include "UnitPawn.h"
#include "CursorActor.h"


// Sets default values
ACursorActor::ACursorActor()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Origin = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComp"));
	RootComponent = Origin;

	
	ArrowComponent = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("ArrowComponent"));
	ArrowComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	UpperLeftComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("UpperLeftComponent"));
	UpperLeftComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	UpperRightComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("UpperRightComponent"));
	UpperRightComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	LowerLeftComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LowerLeftComponent"));
	LowerLeftComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	LowerRightComp = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("LowerRightComponent"));
	LowerRightComp->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->AttachTo(CameraBoom, USpringArmComponent::SocketName);

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->TargetArmLength = 500.0F;
	targetZoom = CameraBoom->TargetArmLength;
}

// Called when the game starts or when spawned
void ACursorActor::BeginPlay()
{
	Super::BeginPlay();
	GetWorld()->GetFirstPlayerController()->Possess(this);

	const FVector Start = GetActorLocation() + FVector(0, 0, 50);
	const FVector End = Start + FVector(0, 0, -100);
	TArray<FHitResult> HitData;

	FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
	RV_TraceParams.bTraceComplex = true;
	RV_TraceParams.bTraceAsyncScene = true;
	RV_TraceParams.bReturnPhysicalMaterial = false;

	GetWorld()->LineTraceMultiByChannel(
		HitData,        //result
		Start,    //start
		End, //end
		ECC_Pawn, //collision channel
		RV_TraceParams
	);

	for (int32 b = 0; b < HitData.Num(); b++)
	{
		if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
		{
			CurrentTile = Cast<AMyTile>(HitData[b].GetActor());
		}

	}
	
}

// Called every frame
void ACursorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//Cursor corner animation
	if (bIsArrowGoingUp)
	{
		if (ArrowComponent->RelativeLocation.Z < 85.F)
		{
			ArrowComponent->SetRelativeLocation((ArrowComponent->RelativeLocation + DeltaTime * FVector(40, 0, 40)));
		}
		else
		{
			bIsArrowGoingUp = false;
		}

	}
	else
	{
		if (ArrowComponent->RelativeLocation.Z > 65.F)
		{
			ArrowComponent->SetRelativeLocation((ArrowComponent->RelativeLocation - DeltaTime * FVector(40, 0, 40)));
		}
		else
		{
			bIsArrowGoingUp = true;
		}

	}

	if (bIsCornersExpanding)
	{
		if (!FMath::IsNearlyEqual(UpperLeftComp->RelativeLocation.X, 30, 9.9999999e-03F))
		{
			UpperLeftComp->SetRelativeLocation(FMath::VInterpTo(UpperLeftComp->RelativeLocation, FVector(30, -30, 0), DeltaTime, 10));
			UpperRightComp->SetRelativeLocation(FMath::VInterpTo(UpperRightComp->RelativeLocation, FVector(30, 30, 0), DeltaTime, 10));
			LowerLeftComp->SetRelativeLocation(FMath::VInterpTo(LowerLeftComp->RelativeLocation, FVector(-30, -30, 0), DeltaTime, 10));
			LowerRightComp->SetRelativeLocation(FMath::VInterpTo(LowerRightComp->RelativeLocation, FVector(-30, 30, 0), DeltaTime, 10));
		}
		else
		{
			bIsCornersExpanding = false;
		}
	}
	else
	{
		if (!FMath::IsNearlyEqual(UpperLeftComp->RelativeLocation.X, 20, 9.9999999e-03F))
		{
			UpperLeftComp->SetRelativeLocation(FMath::VInterpTo(UpperLeftComp->RelativeLocation, FVector(20, -20, 0), DeltaTime, 10));
			UpperRightComp->SetRelativeLocation(FMath::VInterpTo(UpperRightComp->RelativeLocation, FVector(20, 20, 0), DeltaTime, 10));
			LowerLeftComp->SetRelativeLocation(FMath::VInterpTo(LowerLeftComp->RelativeLocation, FVector(-20, -20, 0), DeltaTime, 10));
			LowerRightComp->SetRelativeLocation(FMath::VInterpTo(LowerRightComp->RelativeLocation, FVector(-20, 20, 0), DeltaTime, 10));
		}
		else
		{
			bIsCornersExpanding = true;
		}
	}

	if (GetWorldTimerManager().GetTimerRemaining(LeftRightDelayTimer) > 0 
		|| GetWorldTimerManager().GetTimerRemaining(UpDownDelayTimer) > 0
		|| GetWorldTimerManager().GetTimerRemaining(CurrentTile->InputDelayTimer) > 0)
	{
		FVector2D position(FMath::Vector2DInterpTo(FVector2D(GetActorLocation()), FVector2D(CurrentTile->GetActorLocation()), DeltaTime, 25));
		SetActorLocation(FVector(position.X, position.Y, CurrentTile->GetActorLocation().Z + 20));
		
	}
	else 
	{
		SetActorLocation(FVector(CurrentTile->GetActorLocation().X,
			CurrentTile->GetActorLocation().Y, 
			CurrentTile->GetActorLocation().Z + 20));
	}
	if (targetZoom != CameraBoom->TargetArmLength)
	{
		CameraBoom->TargetArmLength = FMath::FInterpTo(CameraBoom->TargetArmLength, targetZoom, DeltaTime, 25);
	}
	
	
}

// Called to bind functionality to input
void ACursorActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveUp", this, &ACursorActor::MoveUp);
	InputComponent->BindAxis("MoveRight", this, &ACursorActor::MoveRight);

	InputComponent->BindAction("ZoomIn", IE_Pressed, this, &ACursorActor::ZoomIn);
	InputComponent->BindAction("ZoomOut", IE_Pressed, this, &ACursorActor::ZoomOut);
}
void ACursorActor::MoveUp(float Value)
{
	if (!Value == 0 && !GetWorldTimerManager().IsTimerActive(UpDownDelayTimer))
	{
		AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());
		int32 InputDir = (Value > 0 ? 1 : -1);
		if (state->GameplayState == EGameplayStateEnum::EG_BaseMenu || state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
		{
			// find out which way is forward 
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);
			// get forward vector
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

			//Check and see if there is a tile in the specified direction

			const FVector Start = CurrentTile->GetActorLocation() + (CurrentTile->tileSize * InputDir * Direction) + FVector(0, 0, -20);
			const FVector End = Start + FVector(0, 0, 40);
			TArray<FHitResult> HitData;

			FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
			RV_TraceParams.bTraceComplex = true;
			RV_TraceParams.bTraceAsyncScene = true;
			RV_TraceParams.bReturnPhysicalMaterial = false;

			GetWorld()->LineTraceMultiByChannel(
				HitData,        //result
				Start,    //start
				End, //end
				ECC_Pawn, //collision channel
				RV_TraceParams
			);

			for (int32 b = 0; b < HitData.Num(); b++)
			{
				if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
				{
					CurrentTile = Cast<AMyTile>(HitData[b].GetActor());
					GetWorldTimerManager().SetTimer(UpDownDelayTimer, 0.15, false);


					if (state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
					{
						//Check if tile is within any possible movement range
						for (int i = 0; i < AUnitPawn::SelectedUnitPawn->moveRange.Num(); i++)
						{
							if (CurrentTile == AUnitPawn::SelectedUnitPawn->moveRange[i].GetTile())
							{
								if (AUnitPawn::CurrentPath.Num() == 0 && CurrentTile != AUnitPawn::SelectedUnitPawn->currentLocation)
								{
									AUnitPawn::CurrentPath.Add(CurrentTile);
									break;
								}
								else if (AUnitPawn::CurrentPath.Num() <= 1 && CurrentTile == AUnitPawn::SelectedUnitPawn->currentLocation
									|| AUnitPawn::CurrentPath.Num() > 1 && CurrentTile == AUnitPawn::CurrentPath.Last(1))
								{
									AUnitPawn::CurrentPath.Pop();
									break;
								}
								else
								{
									int32 sumCost = 0;
									//Check and see if the total cost is greater than available move
									for (int j = 0; j < AUnitPawn::CurrentPath.Num(); j++)
									{
										sumCost += *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(AUnitPawn::CurrentPath[j]->TileTypeEnum);
									}
									sumCost += *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(CurrentTile->TileTypeEnum);
									if (sumCost <= AUnitPawn::SelectedUnitPawn->GetTotalMovement())
									{
										AUnitPawn::CurrentPath.Add(CurrentTile);
									}
									else
									{
										if (CurrentTile != AUnitPawn::SelectedUnitPawn->currentLocation)
										{
											TArray<AMyTile*> newPath = AUnitPawn::SelectedUnitPawn->moveRange[i].GetShortestPath();
											if (newPath.Num() > 0)
											{
												newPath.RemoveAt(0); //We don't need the base tile in the current path
											}
											AUnitPawn::CurrentPath = newPath;
											AUnitPawn::CurrentPath.Add(CurrentTile);
										}
										else
										{
											AUnitPawn::CurrentPath.Empty();
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void ACursorActor::MoveRight(float Value)
{
	if (Value != 0 && !GetWorldTimerManager().IsTimerActive(LeftRightDelayTimer))
	{
		AMapGameStateBase* state = Cast<AMapGameStateBase>(GetWorld()->GetGameState());

		int32 InputDir = (Value > 0 ? 1 : -1);

		if (state->GameplayState == EGameplayStateEnum::EG_BaseMenu || state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
		{
			// find out which way is forward
			const FRotator Rotation = GetControlRotation();
			const FRotator YawRotation(0, Rotation.Yaw, 0);

			// get right vector 
			const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

			//Check and see if there is a tile in the specified direction

			const FVector Start = CurrentTile->GetActorLocation() + (CurrentTile->tileSize * InputDir * Direction) + FVector(0, 0, 20);
			const FVector End = Start + FVector(0, 0, -40);
			TArray<FHitResult> HitData;

			FCollisionQueryParams RV_TraceParams = FCollisionQueryParams(FName(TEXT("RV_Trace")), true, this);
			RV_TraceParams.bTraceComplex = true;
			RV_TraceParams.bTraceAsyncScene = true;
			RV_TraceParams.bReturnPhysicalMaterial = false;

			GetWorld()->LineTraceMultiByChannel(
				HitData,        //result
				Start,    //start
				End, //end
				ECC_Pawn, //collision channel
				RV_TraceParams
			);

			for (int32 b = 0; b < HitData.Num(); b++)
			{
				if (HitData[b].GetActor()->IsA(AMyTile::StaticClass()))
				{
					CurrentTile = Cast<AMyTile>(HitData[b].GetActor());
					GetWorldTimerManager().SetTimer(LeftRightDelayTimer, 0.15, false);

					if (state->GameplayState == EGameplayStateEnum::EG_MoveMenu)
					{
						//Check if tile is within any possible movement range
						for (int i = 0; i < AUnitPawn::SelectedUnitPawn->moveRange.Num(); i++)
						{
							if (CurrentTile == AUnitPawn::SelectedUnitPawn->moveRange[i].GetTile())
							{
								if (AUnitPawn::CurrentPath.Num() == 0)
								{
									AUnitPawn::CurrentPath.Add(CurrentTile);
									break;
								}
								else if (AUnitPawn::CurrentPath.Num() == 1 && CurrentTile == AUnitPawn::SelectedUnitPawn->currentLocation
									|| AUnitPawn::CurrentPath.Num() > 1 && CurrentTile == AUnitPawn::CurrentPath.Last(1))
								{
									AUnitPawn::CurrentPath.Pop();
									break;
								}
								else
								{
									int32 sumCost = 0;
									//Check and see if the total cost is greater than available move
									for (int j = 0; j < AUnitPawn::CurrentPath.Num(); j++)
									{
										sumCost += *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(AUnitPawn::CurrentPath[j]->TileTypeEnum);
									}
									sumCost += *AUnitPawn::SelectedUnitPawn->CurrentClass->CostOfMove.Find(CurrentTile->TileTypeEnum);
									if (sumCost <= AUnitPawn::SelectedUnitPawn->GetTotalMovement())
									{
										AUnitPawn::CurrentPath.Add(CurrentTile);
									}
									else
									{
										AUnitPawn::CurrentPath.Empty();
										TArray<AMyTile*> newPath = AUnitPawn::SelectedUnitPawn->moveRange[i].GetShortestPath();
										if (newPath.Num() > 0)
										{
											newPath.RemoveAt(0); //We don't need the base tile in the current path
										}
										AUnitPawn::CurrentPath = newPath;
										AUnitPawn::CurrentPath.Add(CurrentTile);
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void ACursorActor::ZoomIn()
{
	if (targetZoom > MIN_ZOOM)
	{
		targetZoom = targetZoom - 100;
		//CameraBoom->AddLocalRotation(FRotator(-2.5, 0, 0));
	}
}

void ACursorActor::ZoomOut()
{
	if (targetZoom < MAX_ZOOM)
	{
		targetZoom += 100;
		//CameraBoom->AddLocalRotation(FRotator(2.5, 0, 0));
	}
}
