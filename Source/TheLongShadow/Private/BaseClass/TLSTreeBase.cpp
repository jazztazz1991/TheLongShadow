// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClass/TLSTreeBase.h"
#include "Engine/DamageEvents.h"

ATLSTreeBase::ATLSTreeBase()
{
    MainTreeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Main Tree Mesh"));
    MainTreeMesh->SetupAttachment(RootComponent);
    TreeStumpMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tree Stump Mesh"));
    TreeStumpMesh->SetupAttachment(RootComponent);
    TreeStumpMesh->bHiddenInGame = true;
    TreeStumpMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ATLSTreeBase::SetHarvestState()
{
    if (!bIsHarvested)
    {
        return;
    }
    // MainTreeMesh->DestroyComponent();
    MainTreeMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    MainTreeMesh->bHiddenInGame = true;
    TreeStumpMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    TreeStumpMesh->bHiddenInGame = false;
}

void ATLSTreeBase::Harvest()
{
    bIsHarvested = true;
    // TODO: Add in Pickup spawn logic here
    SetHarvestState();
    OnHarvestedBP();
}
void ATLSTreeBase::OnHarvestedBP_Implementation()
{
    OnHarvestedBP();
}
FSaveActorData ATLSTreeBase::GetSaveData_Implementation()
{
    FSaveActorData Ret;
    Ret.ActorTransform = GetActorTransform();
    Ret.ActorClass = this->GetClass();
    Ret.WasSpawned = bWasSpawned;
    return Ret;
}
void ATLSTreeBase::UpdateFromSave_Implementation()
{
    SetHarvestState();
}
float ATLSTreeBase::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser)
{
    if(!DamageCauser->Tags.Contains("HarvestTree"))
    {
        return 0.0f;
    }
    Health -= DamageAmount;
    if (Health > 0.0f)
    {
        return 0.0f;
    }
    Harvest();
    return 0.0f;
}
