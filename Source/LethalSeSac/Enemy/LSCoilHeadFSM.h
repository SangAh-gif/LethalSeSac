// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "LSCoilHeadFSM.generated.h"

UENUM(BlueprintType)
enum class ECoilHeadState : uint8
{
	Idle,
	Move,
	Patrol,
	Attack,
	LookAtMe
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class LETHALSESAC_API ULSCoilHeadFSM : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULSCoilHeadFSM();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = FSM)
	ECoilHeadState mState = ECoilHeadState::Idle;

	UPROPERTY();
	class ULSCoilHeadAnim* Anim;

	UPROPERTY(VisibleAnywhere, Category = FSM);
	class ALSCharacter* target;

	UPROPERTY();
	class ALSCoilHead* me;

	UPROPERTY();
	class ALSCoilHeadController* ai;
	//class AAIController* ai;


public:
	FVector randomPos; 

	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float IdleDelayTime = 2.0f;
	
	UPROPERTY(EditDefaultsOnly, Category = FSM)
	float currentTime = 0.0f;

	UPROPERTY(EditAnywhere, Category =FSM)
	float attackDelayTime = 0.5f;

	UPROPERTY(EditAnywhere, Category = FSM)
	float damageDelayTime = 2.0f;

	UPROPERTY(EditAnywhere, Category =FSM)
	float attackRange = 150.0f;

	UPROPERTY(EditAnywhere, Category =FSM)
	float  traceRange = 500.0f;

	FTimerHandle AttackTimerHandle;


public:
	void IdleState();

	void PatrolState();

	void MoveState();

	void AttackState();

	void LookAtMeState();

	bool GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest);

	bool bIsPlayerLooking = false;

	bool blooking = false;

	
public:
	UPROPERTY(EditDefaultsOnly, Category = Sound )
	class USoundBase* CoilHeadStopSound;

	UPROPERTY(EditDefaultsOnly, Category = Sound )
	class USoundBase* CoilHeadMoveSound;

};
