// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Interface/SaveActorInterface.h"
#include "TLSGameInstance.generated.h"

/*
	Transform -
		Location
		Rotation
		Scale

	Health
	Stamina (if put in game)
	Hunger
	Thirst

	Inventory
	Money
	Experience

	WasHarvested
 */
UCLASS()
class THELONGSHADOW_API UTLSGameInstance : public UGameInstance
{
	GENERATED_BODY()

private:
	UPROPERTY()
	TMap<FGuid, FSaveActorData> SaveableActorData;
	UPROPERTY()
	class UTLSSaveGame *SaveGameObject = nullptr;
	UPROPERTY()
	FString SaveGameName = TEXT("DEFAULT");
	UPROPERTY()
	FName CurrentlyLoadedLevel = "NONE";
	UPROPERTY()
	FSaveActorData PlayerData;

	UTLSGameInstance();

	void CreateSaveSlot();
	void GatherActorData();
	void LoadGame();

	void GatherPlayerData();
	void SetPlayerData();

public:
	UFUNCTION(BlueprintCallable)
	void AddActorData(const FGuid &ActorID, FSaveActorData ActorData);
	UFUNCTION(BlueprintCallable)
	FSaveActorData GetActorData(const FGuid &ActorID);

	UFUNCTION(BlueprintCallable)
	void DEV_SaveGame();
	UFUNCTION(BlueprintCallable)
	void DEV_LoadGame();
};
