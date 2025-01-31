// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SaveActorInterface.generated.h"

USTRUCT(BlueprintType)
struct FSaveComponentData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass *ComponentClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FString> RawData;
};

USTRUCT(BlueprintType)
struct FSaveActorData
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<uint8> ByteData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	TArray<FSaveComponentData> ComponentData;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool WasSpawned = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UClass *ActorClass;
};

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USaveActorInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class THELONGSHADOW_API ISaveActorInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent)
	FGuid GetActorSaveID();						   // Blueprint Version of GetActorID
	virtual FGuid GetActorSaveID_Implementation(); // C++ Version of GetActorID
	UFUNCTION(BlueprintNativeEvent)
	void SetActorGUID(const FGuid& NewGuid);
	void SetActorGUID_Implementation(const FGuid& NewGuid);
	UFUNCTION(BlueprintNativeEvent)
	FSaveActorData GetSaveData();						 // Blueprint Version of GetSaveData
	virtual FSaveActorData GetSaveData_Implementation(); // C++ Version of GetSaveData
	UFUNCTION(BlueprintNativeEvent)
	FSaveComponentData GetComponentSaveData();					 // Blueprint Version of GetComponentSaveData
	virtual FSaveComponentData GetComponentSaveData_Implementation(); // C++ Version of GetComponentSaveData
	UFUNCTION(BlueprintNativeEvent)
	void SetComponentSaveData(FSaveComponentData Data); // Blueprint Version of SetComponentSaveData
	virtual void SetComponentSaveData_Implementation(FSaveComponentData Data); // C++ Version of SetComponentSaveData

};
