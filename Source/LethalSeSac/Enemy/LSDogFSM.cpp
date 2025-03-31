// Fill out your copyright notice in the Description page of Project Settings.


#include "LSDogFSM.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LSEyelessDog.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "AITypes.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LSEyelessDogAnim.h"
#include "LSEyelessDogAIController.h"
#include "Components\CapsuleComponent.h"

// Sets default values for this component's properties
ULSDogFSM::ULSDogFSM()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

// Called when the game starts
void ULSDogFSM::BeginPlay()
{
	Super::BeginPlay();

	// ...
	auto actor = UGameplayStatics::GetActorOfClass(GetWorld(), ALSCharacter::StaticClass());

	target = Cast<ALSCharacter>(actor);

	me = Cast<ALSEyelessDog>(GetOwner());

	Anim = Cast<ULSEyelessDogAnim>(me->GetMesh()->GetAnimInstance());

	ai = Cast<ALSEyelessDogAIController>(me->GetController());

}


// Called every frame
void ULSDogFSM::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
	//FString logMsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logMsg);

	switch (mState)
	{
	case EEnemyState::Idle:
	{
		IdleState();
	}
		break;
	case EEnemyState::Move:
	{
		MoveState();
	}
		break;
	case EEnemyState::Attack:
	{
		AttackState();
	}
		break;
	case EEnemyState::Damage:
	{
		DamageState();
	}
		break;
	case EEnemyState::Die:
	{
		DieState();
	}
		break;
	case EEnemyState::Patrol:
	{
		PatrolState();
	}
	break;
	default:
		break;
	}
}

void ULSDogFSM::IdleState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime >= IdleDelayTime)
	{
		mState          = EEnemyState::Patrol;
		currentTime     = 0.0f;
		Anim->AnimState = mState;
		GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
	}
}

void ULSDogFSM::SetNoiseLocation(FVector NewLocation)
{
	NoiseLocation = NewLocation;
	mState = EEnemyState::Move;
}

void ULSDogFSM::MoveState()
{
	if (UKismetMathLibrary::EqualEqual_VectorVector(NoiseLocation, me->GetActorLocation(), 0.00001f))
	{
		IdleState();
	}

	FVector desttination = NoiseLocation;

	me->GetCharacterMovement()->MaxWalkSpeed = 1000.0f;

	FVector dir = desttination - me->GetActorLocation();

	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());

	FPathFindingQuery query;
	FAIMoveRequest req;

	req.SetAcceptanceRadius(7);
	req.SetGoalLocation(desttination);

	ai->BuildPathfindingQuery(req, query);

	FPathFindingResult r = ns->FindPathSync(query);

	if (r.Result == ENavigationQueryResult::Success)
	{
		ai->MoveToLocation(desttination);
	}
	else
	{
		auto result = ai->MoveToLocation(randomPos);

		if (result == EPathFollowingRequestResult::AlreadyAtGoal)
		{
			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		}
	}

	if (dir.Size() < attackRange)
	{
		ai->StopMovement();

		mState = EEnemyState::Attack;

		Anim->AnimState = mState;

		Anim->bAttackPlay = true;

		currentTime = attackDelayTime;
	}
}

void ULSDogFSM::AttackState()
{

	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime > attackDelayTime)
	{
	
		currentTime = 0.0f;
		Anim->bAttackPlay = true;
	}
	else
	{
		float distance = FVector::Distance(target->GetActorLocation(), me->GetActorLocation());

		if (distance > attackRange)
		{
			mState = EEnemyState::Idle;

			Anim->AnimState = mState;

			GetRandomPositionInNavMesh(me->GetActorLocation(), 500.0f, randomPos);
		}
	}
}


void ULSDogFSM::DamageState()
{
	currentTime += GetWorld()->DeltaTimeSeconds;

	if (currentTime >= damageDelayTime)
	{
		mState = EEnemyState::Idle;
		currentTime = 0.0f;

		Anim->AnimState = mState;
	}
}

void ULSDogFSM::DieState()
{
	ai->isDead = true;

	me->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
}

void ULSDogFSM::PatrolState()
{
	me->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	auto result = ai->MoveToLocation(randomPos);
	if (result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		GetRandomPositionInNavMesh(me->GetActorLocation(), 1000.0f, randomPos);
	}
}

bool ULSDogFSM::GetRandomPositionInNavMesh(FVector centerLocation, float radius, FVector& dest)
{
	auto ns = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	
	FNavLocation loc;
	bool result = ns->GetRandomReachablePointInRadius(centerLocation, radius, loc);
	
	dest = loc.Location;

	return result; 
}

void ULSDogFSM::OnDamageProcess(int damage)
{
	hp -= damage;
	if (hp > 0)
	{
		mState = EEnemyState::Damage;
		int32 randValue = FMath::RandRange(0, 1);
		FString sectionName = FString::Printf(TEXT("Damage%d"), randValue);
		me->PlayAnimMontage(Anim->EnemyMongtage, 1.0f, FName(*sectionName));
	}
	else
	{
		mState = EEnemyState::Die;
		me->PlayAnimMontage(Anim->EnemyMongtage, 1.0f, TEXT("Die"));
	}
}

