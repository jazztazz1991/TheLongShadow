// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClass/TLSCharacter.h"
#include "Components/StatlineComponent.h"
#include "TLSCharacter.h"

// Sets default values
ATLSCharacter::ATLSCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Statline = CreateDefaultSubobject<UStatlineComponent>(TEXT("Statline"));
	Statline->SetMovementCompReference(GetCharacterMovement());

	SaveActorID = FGuid::NewGuid();
}

// Called when the game starts or when spawned
void ATLSCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!SaveActorID.IsValid())
	{
		SaveActorID = FGuid::NewGuid();
	}
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

FGuid ATLSCharacter::GetActorSaveID_Implementation()
{
	return SaveActorID;
}

FSaveActorData ATLSCharacter::GetSaveData_Implementation()
{
	FSaveActorData Ret;
	Ret.ActorClass = this->GetClass();
	Ret.ActorTransform = this->GetActorTransform();
	Ret.WasSpawned = this->WasSpawned;
	return Ret;
}

void ATLSCharacter::SetActorGUID_Implementation(const FGuid &NewGuid)
{
	if (SaveActorID.IsValid())
	{
		SaveActorID.Invalidate();
	}
	SaveActorID = NewGuid;
}
