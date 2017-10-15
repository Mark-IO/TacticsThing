/*
	Handles mouse and controller input

*/

#pragma once

#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "PlayerCursor.generated.h"


UCLASS()
class UEOVERWATCHEMBLEM_API APlayerCursor : public APlayerController
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerCursor(const FObjectInitializer& ObjectInitializer);
	FTimerHandle MouseDeltaTimer;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FVector2D LastMousePosition;

public:	
	static FTimerHandle InputDelayTimer;
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupInputComponent() override;

	void ProcessConfirm();

	void ProcessCancel();

	//void ProcessZoomIn();
	//void ProcessZoomOut();
	void ProcessToggleDangerZone();
	
};
