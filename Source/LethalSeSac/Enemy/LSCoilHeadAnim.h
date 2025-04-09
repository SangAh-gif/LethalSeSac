// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "LSCoilHeadFSM.h"
#include "LSCoilHeadAnim.generated.h"

/**
 * 
 */
UCLASS()
class LETHALSESAC_API ULSCoilHeadAnim : public UAnimInstance
{
	GENERATED_BODY()

public:
	ULSCoilHeadAnim();
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = FSM)
	ECoilHeadState AnimState = ECoilHeadState::Idle;

public:
	bool bAttackPlay = false;

	UFUNCTION()
	void AnimNotify_OnSound();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category =FSM)
	class UAnimMontage* MoveMontage;
};
