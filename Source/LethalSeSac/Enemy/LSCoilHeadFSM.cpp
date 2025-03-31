// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCoilHeadFSM.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LSCoilHead.h"
#include "NavigationSystem.h"
#include "Kismet/KismetMathLibrary.h"
#include "AITypes.h"
#include "Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "LSCoilHeadAnim.h"
#include "LSCoilHeadController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"

// Sets default values for this component's properties
ULSCoilHeadFSM::ULSCoilHeadFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void ULSCoilHeadFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...

	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ALSCharacter::StaticClass());
	
	target = Cast<ALSCharacter>(actor);


	me = Cast<ALSCoilHead>(GetOwner());

	Anim = Cast <ULSCoilHeadAnim>(me->GetMesh()->GetAnimInstance());
	

	ai = Cast<ALSCoilHeadController>(me->GetController());
	//ai = Cast<AAIController>(me->GetController());
}


// Called every frame
void ULSCoilHeadFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...

	FString logMsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Blue, logMsg);

	bIsPlayerLooking = ai->IsPlayerLookAtMe();

	if (me->GetMesh()->UPrimitiveComponent::WasRecentlyRendered())
	{
		bIsPlayerLooking = true;
	}
	else
	{
		bIsPlayerLooking = false;
	}
		

	switch (mState)
	{
	case ECoilHeadState::Idle:     { IdleState();     } break;
	case ECoilHeadState::Patrol:   { PatrolState();   } break;
	case ECoilHeadState::Attack:   { AttackState();   } break;
	case ECoilHeadState::Move:     { MoveState();     } break;
	case ECoilHeadState::LookAtMe: { LookAtMeState(); } break;
	}
}

void ULSCoilHeadFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime >= IdleDelayTime)
	{
		mState          = ECoilHeadState::Patrol;
		currentTime     = 0.0f;
		Anim->AnimState = mState;
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
	}
}	

void ULSCoilHeadFSM::MoveState()
{
	FVector desttination = target->GetActorLocation();

	if (!target)
	{
		UE_LOG(LogTemp, Warning, TEXT("Target is null"));
	}

	// 방향 
	FVector dir = desttination - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	// 목적지 길찾기 경로 데이터 검색 
	FPathFindingQuery query;
	FAIMoveRequest req;

	// 목적지에서 인지할 수 있는 범위 
	req.SetAcceptanceRadius(7);
	req.SetGoalLocation(desttination);

	// 길찾기를 위한 쿼리 
	ai->BuildPathfindingQuery(req, query);

	// 길찾기 결과 
	FPathFindingResult r = ns->FindPathSync(query);

	// 목적지까지 길찾기 여부 
	if (r.Result == ENavigationQueryResult::Success)
	{
		// 타겟이동 
		ai->MoveToLocation(desttination); // 안 보고 있다면 움직이게 바꾸기
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos); // 다시 랜덤으로 움직이게  바꾸기

		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			// 새로운 랜덤위치 
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		}
	}

	//if (dir.Size() < attackRange) // 공격 반경에 들어오면 
	//{
	//	ai->StopMovement(); // 움직임을 멈추고 

	//	mState = ECoilHeadState::Attack; // 공격을 한다

	//	Anim->AnimState = mState; // 블프와 연ㄱ동

	//	Anim->bAttackPlay = true;

	//	currentTime = attackDelayTime; // 리셋 
	//}
}

void ULSCoilHeadFSM::PatrolState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	auto result = ai->MoveToLocation(randomPos);
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 1000.0f, randomPos);
		me->bUseControllerRotationYaw = true;
	}
}

void ULSCoilHeadFSM::AttackState()
{
	if (target && FVector::Dist(me->GetActorLocation(), target->GetActorLocation()) < 100.0f)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit"));
	}
}

void ULSCoilHeadFSM::LookAtMeState()
{
	if (bIsPlayerLooking)
	{
		me->GetCharacterMovement()->StopMovementImmediately();
		me->bUseControllerRotationYaw = false;
		//mState = ECoilHeadState::Idle;
	}
	else
	{
		mState = ECoilHeadState::Move;
		ai->MoveToLocation(target->GetActorLocation());
		me->bUseControllerRotationYaw = true;
	}
}

bool ULSCoilHeadFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);

	dest = loc.Location;

	return result;
}

