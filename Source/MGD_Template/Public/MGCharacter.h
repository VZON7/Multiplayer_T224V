// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "MGCharacter.generated.h"

UCLASS()
class MGD_TEMPLATE_API AMGCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMGCharacter();

	// move the character based on an axis
	UFUNCTION(BlueprintCallable, Category="Movement")
	void Pure_MoveCharacter(const FVector2D Axis);

	//rotate the character controller based on axis
	UFUNCTION(BlueprintCallable, Category="Controller")
	void Pure_RotateController(const FVector2D Axis);

protected:
	UFUNCTION(Server, Reliable, Category="Replication")
	void Server_RepPitch(const float& Pitch);

	UFUNCTION(NetMulticast, Reliable, Category="Replication")
	void Multi_RepPitch(const float& Pitch);

public:
	//move axis of the character
	UPROPERTY(BlueprintReadOnly, Category="Movement")
	FVector2D pMoveAxis;

	//replicated pitch for the character
	UPROPERTY(BlueprintReadOnly, Category="Pitch")
	float pRepPitch;

};
