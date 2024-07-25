// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MGGameMode.h"
#include "MGGameMode_Lobby.generated.h"

/**
 * 
 */
UCLASS()
class MGD_TEMPLATE_API AMGGameMode_Lobby : public AMGGameMode
{
	GENERATED_BODY()

	// allows other classes to use that function
public:

	// only this class and classes derived can access this function
protected:
	virtual void OnPostLogin(AController* NewPlayer) override;

	// only this class can access these functions
private:
	
};

	
	
