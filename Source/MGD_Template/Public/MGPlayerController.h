// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MGPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API AMGPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void OnRep_Pawn() override;

	virtual void OnPossess(APawn* InPawn) override;

protected:
	UFUNCTION(BlueprintImplementableEvent, Category="Replication")
	void Pure_OnRepPosess(APawn* PosessedPawn);
};
