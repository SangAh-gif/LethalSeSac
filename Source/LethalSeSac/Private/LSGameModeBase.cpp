// Fill out your copyright notice in the Description page of Project Settings.


#include "LSGameModeBase.h"
#include "ValueSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "LeverActor.h"
#include "SpaceShipActor.h"
#include "LSCharacter.h"


void ALSGameModeBase::BeginPlay()
{
	LoadGameData();
	ALeverActor* Lever = Cast<ALeverActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALeverActor::StaticClass()));
	ASpaceShipActor* SpaceShip = Cast<ASpaceShipActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpaceShipActor::StaticClass()));
	ALSCharacter* player = Cast<ALSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
	if (Lever)
	{
		Lever->bInSpace = bInSpace;
	}
	if (SpaceShip)
	{
		SpaceShip->totVal = TotValue;
		SpaceShip->Quota = Quota;
	}
	if (bInSpace) 
	{
		if (GameDay <= 0)
		{
			if (Quota > TotValue)
			{
				player->bSucceed = false;
			}
			else
			{
				player->bSucceed = true;
			}
			ResetGameData();
			player->WinGame();
		}
	}
	
}

void ALSGameModeBase::SaveGameData()
{
	UValueSaveGame* sg = Cast<UValueSaveGame>(UGameplayStatics::CreateSaveGameObject(UValueSaveGame::StaticClass()));

	sg->TotValueSave = TotValue;
	sg->bInSpace = bInSpace;
	sg->CurGameDay = GameDay;

	UGameplayStatics::SaveGameToSlot(sg, SaveSlotName, UserIndex);
}

void ALSGameModeBase::LoadGameData()
{
	bool sgExist = UGameplayStatics::DoesSaveGameExist(SaveSlotName, UserIndex);

	if(!sgExist) return;

	UValueSaveGame* sg = Cast<UValueSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, UserIndex));

	if(!sg) return;

	TotValue = sg->TotValueSave;
	bInSpace = sg->bInSpace;
	GameDay = sg->CurGameDay;
}

void ALSGameModeBase::ResetGameData()
{
	TotValue = DefaultTotValueSave;
	bInSpace = bDefaultInSpace;
	GameDay = DefaultCurGameDay;

	SaveGameData();
}