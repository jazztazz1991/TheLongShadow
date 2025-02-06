// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/TLSActor.h"
#include "TLSTreeBase.generated.h"

class UStaticMechComponent;
/**
 *
 */
UCLASS()
class THELONGSHADOW_API ATLSTreeBase : public ATLSActor
{
	GENERATED_BODY()

private:
	UPROPERTY(SaveGame)
	bool bIsHarvested = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float Health = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *MainTreeMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent *TreeStumpMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class AActor *LogPickupActor; // TODO: Change to pickup actor once inventory is implemented
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int NumberOfLogsToSpawn = 2;

	ATLSTreeBase();
	void SetHarvestState();
	void Harvest();

protected:
public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnHarvestedBP();
	void OnHarvestedBP_Implementation();

	virtual FSaveActorData GetSaveData_Implementation() override;
	void UpdateFromSave_Implementation() override;

	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
};
