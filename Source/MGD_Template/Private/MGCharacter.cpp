// Fill out your copyright notice in the Description page of Project Settings.


#include "MGCharacter.h"

// Sets default values
AMGCharacter::AMGCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AMGCharacter::Pure_MoveCharacter(const FVector2D Axis)
{
	//update the member axis
	pMoveAxis = Axis;

	//move character based on Y Axis
	AddMovementInput(GetControlRotation().Vector(), Axis.Y);

	//move character based on X Axis
	AddMovementInput(FRotationMatrix(GetControlRotation()).GetScaledAxis(EAxis::Y), Axis.X);
}

void AMGCharacter::Pure_RotateController(const FVector2D Axis)
{
	//rotate the controller Yaw based on X Axis
	AddControllerYawInput(Axis.X);

	//rotate the controller Pitch based on Y Axis
	AddControllerPitchInput(-Axis.Y);
}
