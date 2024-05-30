// Fill out your copyright notice in the Description page of Project Settings.


#include "MGWeapon.h"

// Sets default values
AMGWeapon::AMGWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMGWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMGWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

