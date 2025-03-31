// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LETHALSESAC_API ALSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GameDay = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quota = 200;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 TotValue = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bInSpace = true;

	UPROPERTY(BlueprintReadWrite, Category = "Default|SaveGame")
	int32 DefaultCurGameDay = 1;
	UPROPERTY(BlueprintReadWrite, Category = "Default|SaveGame")
	int32 DefaultTotValueSave = 0;
	UPROPERTY(BlueprintReadWrite, Category = "Default|SaveGame")
	bool bDefaultInSpace = true;
	UPROPERTY(BlueprintReadWrite, Category = "Default|SaveGame")
	int32 DefaultQuotaSave = 200;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString SaveSlotName = TEXT("SaveValue");
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 UserIndex = 0;

	void SaveGameData();
	void LoadGameData();
	void ResetGameData();

};
