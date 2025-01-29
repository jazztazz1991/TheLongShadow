// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StatlineComponent.generated.h"

USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float Current = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float Max = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1;

public:
	void TickStat(const float &DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0.0f, Max);
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THELONGSHADOW_API UStatlineComponent : public UActorComponent
{
	GENERATED_BODY()

	// Variables needed for the StatlineComponent
	// Health
	//    float currentHealth
	//    float maxHealth
	//    float regenRate
	// Stamina?
	//    float currentStamina
	//    float maxStamina
	//    float regenRate
	// Hunger
	//    float currentHunger
	//    float maxHunger
	//    float regenRate
	// Thirst
	//    float currentThirst
	//    float maxThirst
	//    float regenRate
	// Energy
	//    float currentEnergy
	//    float maxEnergy
	//    float regenRate
	// Experience
	//    float currentExperience
	//    float nextLevelExperience
	//    float experienceMultiplier
	//    float experienceToLevel
	// Level
	//    int currentLevel
	//    int maxLevel

public:
	// Sets default values for this component's properties
	UStatlineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;
};
