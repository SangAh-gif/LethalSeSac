// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LSDogFSM.h"
#include "LSEyelessDogAnim.generated.h"

/**
 * 
 */
UCLASS()
class LETHALSESAC_API ULSEyelessDogAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULSEyelessDogAnim();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	EEnemyState AnimState = EEnemyState::Idle;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	bool bAttackPlay = false; 
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	class UAnimMontage* EnemyMongtage; 

	UFUNCTION()
	void AnimNotify_AttackEnd();

	UFUNCTION()
	void AnimNotify_DieEnd();

public:
	UPROPERTY(EditDefaultsOnly, Category = Sound )
	class USoundBase* DogPtrolSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound )
	class USoundBase* DogMoveSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound )
	class USoundBase* DogAttackSound;
};
