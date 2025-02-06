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
private:
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FGuid SaveID;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bWasSpawned = false;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATLSActor();
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual FSaveActorData GetSaveData_Implementation();
	void SetActorGUID_Implementation(const FGuid &NewGuid);
	virtual FGuid GetActorSaveID_Implementation();
	virtual void UpdateFromSave_Implementation();
};
