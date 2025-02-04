// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/SaveActorInterface.h"
#include "TLSActor.generated.h"

UCLASS()
class THELONGSHADOW_API ATLSActor : public AActor, public ISaveActorInterface
{
	GENERATED_BODY()
	
	FGuid SaveID;

public:
	// Sets default values for this actor's properties
	ATLSActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FSaveActorData GetSaveData_Implementation();
	void SetActorGUID_Implementation(const FGuid &NewGuid);
	virtual FGuid GetActorSaveID_Implementation();
};
