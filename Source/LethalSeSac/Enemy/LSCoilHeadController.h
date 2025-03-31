// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "Perception/AIPerceptionComponent.h"
#include "LSCoilHeadController.generated.h"

/**
 * 
 */

 UENUM()
	 enum class enemyAISenseCoilHead
 {
	Damage,
	Sight
 };

UCLASS()
class LETHALSESAC_API ALSCoilHeadController : public AAIController
{
	GENERATED_BODY()

public:
	ALSCoilHeadController();

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAIPerceptionComponent* PerceptionComp; // 감각 시스템 

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAISenseConfig_Damage* DamageConfig;

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	bool GetActorPerception(AActor* Actor, FActorPerceptionBlueprintInfo& Info);

	bool CanSenseActor(AActor* Actor, enemyAISenseCoilHead AIPerceptionSense);

	bool isDead = false;

	bool IsPlayerLookAtMe();

	void MoveToRandomLocation();

	UPROPERTY(VisibleAnywhere, Category = FSM)
	class ALSCharacter* target;

	UPROPERTY()
	class ALSCoilHead* me;

};
