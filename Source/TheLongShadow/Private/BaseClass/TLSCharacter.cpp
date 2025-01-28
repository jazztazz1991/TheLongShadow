// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseClass/TLSCharacter.h"

// Sets default values
ATLSCharacter::ATLSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATLSCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATLSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATLSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

