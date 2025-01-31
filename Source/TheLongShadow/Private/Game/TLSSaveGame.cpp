// Fill out your copyright notice in the Description page of Project Settings.

#include "Game/TLSSaveGame.h"
#include "Interface/SaveActorInterface.h"

void UTLSSaveGame::SetSaveActorData(TMap<FGuid, FSaveActorData> Data)
{
    SaveableActorData = Data;
}

TMap<FGuid, struct FSaveActorData> UTLSSaveGame::GetSaveActorData()
{
    return SaveableActorData;
}
void UTLSSaveGame::SetCurrentLevel(const FName Level)
{
    CurrentlyLoadedLevel = Level;
}

FName UTLSSaveGame::GetCurrentLevel()
{
    return CurrentlyLoadedLevel;
}
