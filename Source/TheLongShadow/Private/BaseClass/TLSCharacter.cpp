// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClass/TLSCharacter.h"
#include "Components/StatlineComponent.h"

// Sets default values
ATLSCharacter::ATLSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementCompReference(GetCharacterMovement());
}

// Called when the game starts or when spawned
void ATLSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

bool ATLSCharacter::CanJump() const
{
	return Statline->CanJump();
}
void ATLSCharacter::HasJumped()
{
	Statline->HasJumped();
	ACharacter::Jump();
}
// Called every frame
bool ATLSCharacter::CanSprint() const
{
	return Statline->CanSprint();
}
void ATLSCharacter::SetSprinting(const bool &IsSprinting)
{
	Statline->SetSprinting(IsSprinting);
}
void ATLSCharacter::SetSneaking(const bool &IsSneaking)
{
	Statline->SetSneaking(IsSneaking);
}
void ATLSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ATLSCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}
