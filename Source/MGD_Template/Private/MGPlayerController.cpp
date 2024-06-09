// Fill out your copyright notice in the Description page of Project Settings.


#include "MGPlayerController.h"

void AMGPlayerController::OnRep_Pawn()
{
	Super::OnRep_Pawn();

	Pure_OnRepPosess(GetPawn());
	
}

void AMGPlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	Pure_OnRepPosess(InPawn);
}
