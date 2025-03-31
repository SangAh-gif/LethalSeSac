// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "LSEyelessDogAIController.generated.h"

/**
 * 
 */

 UENUM()
	 enum class enemyAISenseEyelessDog
 {
	Damage,
	Hearing
 };

UCLASS()
class LETHALSESAC_API ALSEyelessDogAIController : public AAIController
{
	GENERATED_BODY()

public:
	ALSEyelessDogAIController();

public:

	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAIPerceptionComponent* PerceptionComp; // 감각 시스템
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAISenseConfig_Hearing* HearingConfig;  // 소리감지 설정

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UAISenseConfig_Damage* DamageConfig;   

	UFUNCTION()
	void PerceptionUpdated(const TArray<AActor*>& UpdatedActors);

	bool CanSenseActor(AActor* actor, enemyAISenseEyelessDog AIPerceptionSense);

	bool isDead = false;

	ALSEyelessDogAIController* SetNoisiLocation(FVector InNoiseLocation) { this->NoiseLocation = InNoiseLocation; return this; }

	void OnHearNoise(APawn* NoiseInstigator, const FVector& Location, float Volume);

	FVector GetNoisLocation();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FVector NoiseLocation; 

	bool bHearNoise = false;

	class ULSDogFSM* dogFSM;
};
