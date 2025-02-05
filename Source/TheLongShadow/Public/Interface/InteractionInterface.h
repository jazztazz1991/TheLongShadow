// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractionInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 *
 */
class THELONGSHADOW_API IInteractionInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	FText GetInteractionText();
	virtual FText GetInteractionText_Implementation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Interact(class ATLSCharacter *Caller);
	void Interact_Implemantation(class ATLSCharacter *Caller);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool isInteractable() const;
	bool isInteractable_Implementation() const;
};
