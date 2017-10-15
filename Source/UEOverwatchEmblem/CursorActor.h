// This is the actual cursor that the player interacts with to select units etc.

#pragma once

#include "GameFramework/Pawn.h"
#include "MyTile.h"
#include "PaperSpriteComponent.h"
#include "CursorActor.generated.h"

UCLASS()
class UEOVERWATCHEMBLEM_API ACursorActor : public APawn
{
	GENERATED_BODY()

	FTimerHandle UpDownDelayTimer;
	FTimerHandle LeftRightDelayTimer;
	FTimerHandle CameraDelayTimer;
	bool bIsArrowGoingUp = true;
	bool bIsCornersExpanding = true;
public:

	/** Follow camera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class UCameraComponent* FollowCamera;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		class USpringArmComponent* CameraBoom;
		
	float const MIN_ZOOM = 400;
		
	float const MAX_ZOOM = 900;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
		float targetZoom;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class USceneComponent* Origin;

	AMyTile* CurrentTile; //Current tile cursor is over
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class UPaperSpriteComponent* ArrowComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class UPaperSpriteComponent* UpperLeftComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class UPaperSpriteComponent* UpperRightComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class UPaperSpriteComponent* LowerLeftComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Testing)
		class UPaperSpriteComponent* LowerRightComp;
	
	// Sets default values for this pawn's properties
	ACursorActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveUp(float Value);
	void MoveRight(float Value);
	void ZoomIn();
	void ZoomOut();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
