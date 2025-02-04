// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClass/TLSActor.h"

// Sets default values
ATLSActor::ATLSActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SaveID = FGuid::NewGuid();
}

// Called when the game starts or when spawned
void ATLSActor::BeginPlay()
{
	Super::BeginPlay();
	if (!SaveID.IsValid())
	{
		SaveID = FGuid::NewGuid();
	}
}

// Called every frame
void ATLSActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FSaveActorData ATLSActor::GetSaveData_Implementation()
{
	return FSaveActorData();
}

void ATLSActor::SetActorGUID_Implementation(const FGuid &NewGuid)
{
	if (SaveID.IsValid())
	{
		SaveID.Invalidate();
	}
}

FGuid ATLSActor::GetActorSaveID_Implementation()
{
	return SaveID;
}
