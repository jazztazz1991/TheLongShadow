// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseClass/TLSActor.h"
#include "Interface/InteractionInterface.h"
#include "TLSDoorBase.generated.h"

/**
 *
 */
UCLASS()
class THELONGSHADOW_API ATLSDoorBase : public ATLSActor, public IInteractionInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsOpen = false;
public:

	ATLSDoorBase();

	FText GetInteractionText_Implementation();
	void Interact_Implemantation(class ATLSCharacter *Caller);
	bool isInteractable_Implementation() const;
};
