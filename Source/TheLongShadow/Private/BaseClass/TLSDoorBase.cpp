// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClass/TLSDoorBase.h"

ATLSDoorBase::ATLSDoorBase()
{
    PrimaryActorTick.bCanEverTick = false;
}
FText ATLSDoorBase::GetInteractionText_Implementation()
{
    return bIsOpen ? FText::FromString("Close") : FText::FromString("Open");
}

void ATLSDoorBase::Interact_Implemantation(ATLSCharacter *Caller)
{
    Interact(Caller);
}

bool ATLSDoorBase::isInteractable_Implementation() const
{
    return true;
}
