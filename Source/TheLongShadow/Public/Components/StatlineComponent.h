// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interface/SaveActorInterface.h"
#include "StatlineComponent.generated.h"

UENUM(BlueprintType)
enum class ECoreStat : uint8
{
	CS_HEALTH UMETA(DisplayName = "Health"),
	// CS_STAMINA UMETA(DisplayName = "Stamina"),
	CS_HUNGER UMETA(DisplayName = "Hunger"),
	CS_THIRST UMETA(DisplayName = "Thirst")

};

USTRUCT(BlueprintType)
struct FCoreStat
{
	GENERATED_USTRUCT_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, SaveGame, meta = (AllowPrivateAccess = "true"))
	float Current = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, SaveGame, meta = (AllowPrivateAccess = "true"))
	float Max = 100.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadonly, SaveGame, meta = (AllowPrivateAccess = "true"))
	float PerSecondTick = 1.0f;

public:
	FCoreStat() {};
	FCoreStat(const float &current, const float &max, const float &tick)
	{
		Current = current;
		Max = max;
		PerSecondTick = tick;
	};
	void TickStat(const float &DeltaTime)
	{
		Current = FMath::Clamp(Current + (PerSecondTick * DeltaTime), 0.0f, Max);
	};
	void Adjust(const float &amount)
	{
		Current = FMath::Clamp(Current + amount, 0.0f, Max);
	};
	float Percentile() const
	{
		return Current / Max;
	};
	void AdjustTick(const float &NewTick)
	{
		PerSecondTick = NewTick;
	};
	float GetCurrent() const
	{
		return Current;
	};

	FString GetSaveString()
	{
		FString Ret = FString::SanitizeFloat(Current);
		Ret += "|";
		Ret += FString::SanitizeFloat(Max);
		Ret += "|";
		Ret += FString::SanitizeFloat(PerSecondTick);
		return Ret;
	}

	void UpdateFromSaveString(TArray<FString> Parts)
	{
		if (Parts.Num() != 3)
		{
			// TODO: Log error
			return;
		}
		Current = FCString::Atof(*Parts[0]);
		Max = FCString::Atof(*Parts[1]);
		PerSecondTick = FCString::Atof(*Parts[2]);
	}
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class THELONGSHADOW_API UStatlineComponent : public UActorComponent, public ISaveActorInterface
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

private:
	class UCharacterMovementComponent *OwningCharacterMovementComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", SaveGame, meta = (AllowPrivateAccess = "true"))
	FCoreStat Health;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	// FCoreStat Stamina;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", SaveGame, meta = (AllowPrivateAccess = "true"))
	FCoreStat Hunger = FCoreStat(100, 100, -0.025);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", SaveGame, meta = (AllowPrivateAccess = "true"))
	FCoreStat Thirst = FCoreStat(100, 100, -0.05);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	bool bIsSneaking = false;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	// float SprintCostMultiplier = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float WalkSpeed = 125;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float SprintSpeed = 450;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float SneakSpeed = 75;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	// float JumpCost = 0;
	// UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	// float SecondsForStaminaExhaustion = 5;
	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	// float CurrentStaminaExhaustion = 0;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float StarvingHealthDamagePerSecond = 1;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Stats", meta = (AllowPrivateAccess = "true"))
	float DehydrationHealthDamagePerSecond = 1;

	void TickStats(const float &DeltaTime);
	// void TickStamina(const float &DeltaTime);
	bool TickHunger(const float &DeltaTime);
	bool TickThirst(const float &DeltaTime);
	bool IsValidSprinting();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Sets default values for this component's properties
	UStatlineComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SetMovementCompReference(class UCharacterMovementComponent *Comp);

	UFUNCTION(BlueprintCallable)
	float GetStatPercentile(const ECoreStat Stat) const;

	UFUNCTION(BlueprintCallable)
	bool CanSprint() const;
	UFUNCTION(BlueprintCallable)
	void SetSprinting(const bool &IsSprinting);
	UFUNCTION(BlueprintCallable)
	void SetSneaking(const bool &IsSneaking);
	UFUNCTION(BlueprintCallable)
	bool CanJump();
	UFUNCTION(BlueprintCallable)
	void HasJumped();

	virtual FSaveComponentData GetComponentSaveData_Implementation();
	virtual void SetComponentSaveData_Implementation(FSaveComponentData Data);
};
