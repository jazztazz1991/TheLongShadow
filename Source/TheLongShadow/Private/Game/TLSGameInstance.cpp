// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TLSGameInstance.h"
#include "Game/TLSSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include <Serialization/ObjectAndNameAsStringProxyArchive.h>

UTLSGameInstance::UTLSGameInstance()
{
}
void UTLSGameInstance::CreateSaveSlot()
{
    SaveGameObject = Cast<UTLSSaveGame>(UGameplayStatics::CreateSaveGameObject(UTLSSaveGame::StaticClass()));
}

void UTLSGameInstance::GatherActorData()
{
    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor *Actor = *It;
        if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
        {
            continue;
        }
        ISaveActorInterface *Inter = Cast<ISaveActorInterface>(Actor);
        if (Inter == nullptr)
        {
            continue;
        }
        // FGuid ActID =  Inter->GetActorID();
        // SAI = SaveActorID
        FGuid SAI = Inter->GetActorSaveID_Implementation();
        if (!SAI.IsValid())
        {
            continue;
        }
        // FSaveActorData = Inter->GetSaveData();
        // SAD = SaveActorData
        FSaveActorData SAD = Inter->GetSaveData_Implementation();

        FMemoryWriter MemWriter(SAD.ByteData);

        FObjectAndNameAsStringProxyArchive Ar(MemWriter, true);
        Ar.ArIsSaveGame = true;
        Actor->Serialize(Ar);

        for (auto ActorComp : Actor->GetComponents())
        {
            if (!ActorComp->Implements<USaveActorInterface>())
            {
                continue;
            }
            ISaveActorInterface *CompInter = Cast<ISaveActorInterface>(ActorComp);
            if (CompInter == nullptr)
            {
                continue;
            }
            FSaveComponentData SCD = CompInter->GetComponentSaveData_Implementation();
            FMemoryWriter CompMemWriter(SCD.ByteData);
            FObjectAndNameAsStringProxyArchive CAr(CompMemWriter, true);
            CAr.ArIsSaveGame = true;
            ActorComp->Serialize(CAr);
            SCD.ComponentClass = ActorComp->GetClass();

            SAD.ComponentData.Add(SCD);
        }

        SaveableActorData.Add(SAI, SAD);
    }
}

void UTLSGameInstance::LoadGame()
{
    if (!UGameplayStatics::DoesSaveGameExist(SaveGameName, 0))
    {
        // TODO: Add loggin and error message about missing save game
        return;
    }

    SaveableActorData.Empty();
    SaveGameObject = Cast<UTLSSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveGameName, 0));
    SaveableActorData = SaveGameObject->GetSaveActorData();

    for (TTuple<FGuid, FSaveActorData> SAD : SaveableActorData)
    {
        if (SAD.Value.WasSpawned)
        {
            AActor *SpawnedActor = GetWorld()->SpawnActor(SAD.Value.ActorClass->StaticClass(), &SAD.Value.ActorTransform);
            ISaveActorInterface *Inter = Cast<ISaveActorInterface>(SpawnedActor);
            if (Inter == nullptr)
            {
                continue;
            }
            // Set Actor GUID
        }
    }

    for (FActorIterator It(GetWorld()); It; ++It)
    {
        AActor *Actor = *It;
        if (!IsValid(Actor) || !Actor->Implements<USaveActorInterface>())
        {
            continue;
        }
        ISaveActorInterface *Inter = Cast<ISaveActorInterface>(Actor);
        if (Inter == nullptr)
        {
            continue;
        }
        FGuid SAI = Inter->GetActorSaveID_Implementation();
        if (!SaveableActorData.Find(SAI))
        {
            continue;
        }
        FSaveActorData SAD = SaveableActorData[SAI];
        Actor->SetActorTransform(SAD.ActorTransform);
        FMemoryReader MemReader(SAD.ByteData);
        FObjectAndNameAsStringProxyArchive Ar(MemReader, true);
        Ar.ArIsSaveGame = true;
        Actor->Serialize(Ar);

        for (auto ActorComp : Actor->GetComponents())
        {
            if (!ActorComp->Implements<USaveActorInterface>())
            {
                continue;
            }
            ISaveActorInterface *CompInter = Cast<ISaveActorInterface>(ActorComp);
            if (CompInter == nullptr)
            {
                continue;
            }
            for (auto SCD : SAD.ComponentData)
            {
                /***************************************************************************
                 *This is not safe if an actor has 2 of the same components, that are saved*
                 *as the 1st component returned by Actor->GetComponents() will get all data*
                 *                                                                         *
                 *     One possible option is a GUID on the component                      *
                 **************************************************************************/
                if (SCD.ComponentClass != ActorComp->GetClass())
                {
                    continue;
                }

                FMemoryReader CompMemReader(SCD.ByteData);
                FObjectAndNameAsStringProxyArchive CAr(CompMemReader, true);
                CAr.ArIsSaveGame = true;
                ActorComp->Serialize(CAr);
                if (SCD.RawData.IsEmpty())
                {
                    break;
                }
                CompInter->SetComponentSaveData_Implementation(SCD);
                break;
            }
        }
    }
}
void UTLSGameInstance::AddActorData(const FGuid &ActorID, FSaveActorData ActorData)
{
    SaveableActorData.Add(ActorID, ActorData);
}

FSaveActorData UTLSGameInstance::GetActorData(const FGuid &ActorID)
{

    return SaveableActorData[ActorID];
}

void UTLSGameInstance::DEV_SaveGame()
{
    if (SaveGameObject == nullptr)
    {
        CreateSaveSlot();
    }
    GatherActorData();
    SaveGameObject->SetSaveActorData(SaveableActorData);
    UGameplayStatics::SaveGameToSlot(SaveGameObject, SaveGameName, 0);
}

void UTLSGameInstance::DEV_LoadGame()
{
    LoadGame();
}
