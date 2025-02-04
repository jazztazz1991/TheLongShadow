// Fill out your copyright notice in the Description page of Project Settings.

#include "Components/StatlineComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TLSUtils.h"
#include "Logger.h"

void UStatlineComponent::TickStats(const float &DeltaTime)
{
	// TickStamina(DeltaTime);
	TickHunger(DeltaTime);
	TickThirst(DeltaTime);
	if (TickHunger(DeltaTime) || TickThirst(DeltaTime))
	{
		return;
	}
	Health.TickStat(DeltaTime);
};
// void UStatlineComponent::TickStamina(const float &DeltaTime)
// {
// 	if (CurrentStaminaExhaustion > 0)
// 	{
// 		CurrentStaminaExhaustion -= DeltaTime;
// 		if (CurrentStaminaExhaustion <= 0)
// 		{
// 			SetSprinting(false);
// 			CurrentStaminaExhaustion = 0;
// 		}
// 		return;
// 	}

// 	if (bIsSprinting && IsValidSprinting())
// 	{
// 		Stamina.TickStat(0 - abs(DeltaTime * SprintCostMultiplier));
// 		if (Stamina.GetCurrent() <= 0)
// 		{
// 			SetSprinting(false);
// 			CurrentStaminaExhaustion = SecondsForStaminaExhaustion;
// 		}
// 		return;
// 	}

// 	Stamina.TickStat(DeltaTime);
// };
bool UStatlineComponent::IsValidSprinting()
{
	return OwningCharacterMovementComp->Velocity.Length() > WalkSpeed && !OwningCharacterMovementComp->IsFalling();
};
// Sets default values for this component's properties
bool UStatlineComponent::TickHunger(const float &DeltaTime)
{
	if (Hunger.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(StarvingHealthDamagePerSecond * DeltaTime));
		return true;
	}

	Hunger.TickStat(DeltaTime);
	return false;
}
bool UStatlineComponent::TickThirst(const float &DeltaTime)
{
	if (Thirst.GetCurrent() <= 0.0)
	{
		Health.Adjust(0 - abs(DehydrationHealthDamagePerSecond * DeltaTime));
		return true;
	}

	Thirst.TickStat(DeltaTime);
	return false;
}
UStatlineComponent::UStatlineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
};

// Called when the game starts
void UStatlineComponent::BeginPlay()
{
	Super::BeginPlay();
	OwningCharacterMovementComp->MaxWalkSpeed = WalkSpeed;
	// ...
};

// Called every frame
void UStatlineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (TickType != ELevelTick::LEVELTICK_PauseTick)
	{
		TickStats(DeltaTime);
	}
}

void UStatlineComponent::SetMovementCompReference(UCharacterMovementComponent *Comp)
{
	OwningCharacterMovementComp = Comp;
}
float UStatlineComponent::GetStatPercentile(const ECoreStat Stat) const
{
	switch (Stat)
	{
	case ECoreStat::CS_HEALTH:
		return Health.Percentile();
	case ECoreStat::CS_HUNGER:
		return Hunger.Percentile();
	case ECoreStat::CS_THIRST:
		return Thirst.Percentile();
	// case ECoreStat::CS_STAMINA:
	// 	return Stamina.Percentile();
	default:
		Logger::GetInstance()->AddMessage("GetStatPercentile called with invalid stat type", ERRORLEVEL::EL_WARNING);
		break;
	}
	return -1;
}

bool UStatlineComponent::CanSprint() const
{
	return true;
}

void UStatlineComponent::SetSprinting(const bool &IsSprinting)
{
	bIsSprinting = IsSprinting;
	if (bIsSneaking && !bIsSprinting)
	{
		return;
	}
	bIsSneaking = false;
	OwningCharacterMovementComp->MaxWalkSpeed = bIsSprinting ? SprintSpeed : WalkSpeed;
}

void UStatlineComponent::SetSneaking(const bool &IsSneaking)
{
	bIsSneaking = IsSneaking;
	if (bIsSprinting && !bIsSneaking)
	{
		return;
	}
	bIsSprinting = false;
	OwningCharacterMovementComp->MaxWalkSpeed = bIsSneaking ? SneakSpeed : WalkSpeed;
}
bool UStatlineComponent::CanJump()
{
	return true;
}

void UStatlineComponent::HasJumped()
{
}

FSaveComponentData UStatlineComponent::GetComponentSaveData_Implementation()
{
	FSaveComponentData Ret;
	Ret.ComponentClass = this->GetClass();
	Ret.RawData.Add(Health.GetSaveString());
	Ret.RawData.Add(Hunger.GetSaveString());
	Ret.RawData.Add(Thirst.GetSaveString());
	// Ret.RawData.Add(Stamina.GetSaveString());

	return Ret;
}

void UStatlineComponent::SetComponentSaveData_Implementation(FSaveComponentData Data)
{
	TArray<FString> Parts;
	for (int i = 0; i < Data.RawData.Num(); i++)
	{
		Parts.Empty();
		Parts = ChopString(Data.RawData[i], '|');
		switch (i)
		{
		case 0:
			Health.UpdateFromSaveString(Parts);
			break;
		case 1:
			Hunger.UpdateFromSaveString(Parts);
			break;
		case 2:
			Thirst.UpdateFromSaveString(Parts);
			break;
		// case 3:
		// 	Stamina.UpdateFromSaveString(Parts);
		default:
			// Log error
			break;
		}
	}
}
