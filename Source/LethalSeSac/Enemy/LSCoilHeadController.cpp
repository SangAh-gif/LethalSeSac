// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCoilHeadController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LSCoilHead.h"
#include "Perception/AIPerceptionSystem.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LSCoilHeadFSM.h"
#include "NavigationSystem.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Damage.h"
#include "LSCoilHeadAnim.h"

ALSCoilHeadController::ALSCoilHeadController()
{
	/*PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*PerceptionComp);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SetMaxAge(3.0f);

	if (SightConfig)
	{
		SightConfig->SightRadius                  = 1000.0f;
		SightConfig->LoseSightRadius              = 1200.0f;
		SightConfig->PeripheralVisionAngleDegrees = 45.0f;

		PerceptionComp->ConfigureSense(*SightConfig);
		PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
	}


	PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ALSCoilHeadController::PerceptionUpdated);*/
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*PerceptionComp);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SightRadius = 1000.0f;
    SightConfig->LoseSightRadius = 1500.0f;
    SightConfig->PeripheralVisionAngleDegrees = 180.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = -1.0f;

    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;

    PerceptionComp->ConfigureSense(*SightConfig);
    PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());

    DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));
    DamageConfig->SetMaxAge(3.0f);
    PerceptionComp->ConfigureSense(*DamageConfig);

    PerceptionComp->OnPerceptionUpdated.AddDynamic(this, &ALSCoilHeadController::PerceptionUpdated);
}

void ALSCoilHeadController::BeginPlay()
{
    Super::BeginPlay();

    AActor* PlayerActor = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    if (PlayerActor)
    {
        target = Cast<ALSCharacter>(PlayerActor);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("타겟을 찾을 수 없습니다."));
    }
}

void ALSCoilHeadController::PerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
    //if(!PerceptionComp) return;

    //for (AActor* UpdatedActor : UpdatedActors)
    //{
    //    if (!UpdatedActor->ActorHasTag("enemy"))
    //    {
    //        FActorPerceptionBlueprintInfo Info;
    //        PerceptionComp->GetActorsPerception(UpdatedActor, Info);

    //        for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
    //        {
    //            if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
    //            {
    //                if (Stimulus.WasSuccessfullySensed())
    //                {
    //                    coilHeadFSM->mState = ECoilHeadState::LookAtMe;
    //                }
    //                else
    //                {
    //                    coilHeadFSM->mState = ECoilHeadState::Patrol;
    //                }
    //                /*ALSCoilHead* enemy = Cast<ALSCoilHead>(GetCharacter());
    //                if (enemy)
    //                {
    //                    ULSCoilHeadFSM* FSM = enemy->FindComponentByClass<ULSCoilHeadFSM>();
    //                    if (FSM)
    //                    {
    //                        FSM->LookAtMeState();
    //                    }
    //                }*/
    //            }
    //        }
    //    }
    //}

    //if (!PerceptionComp) return;

    //for (AActor* UpdatedActor : UpdatedActors)
    //{
    //    if (!UpdatedActor->ActorHasTag("enemy")) // 적이 아닐 때만 반응
    //    {
    //        FActorPerceptionBlueprintInfo Info;
    //        PerceptionComp->GetActorsPerception(UpdatedActor, Info);

    //        for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
    //        {
    //            if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
    //            {
    //                ALSCoilHead* enemy = Cast<ALSCoilHead>(GetCharacter());
    //                if (enemy)
    //                {
    //                    ULSCoilHeadFSM* FSM = enemy->FindComponentByClass<ULSCoilHeadFSM>();
    //                    if (FSM)
    //                    {
    //                        FSM->LookAtMeState();
    //                    }
    //                }
    //            }
    //        }
    //    }
    //}

    /*for (AActor* UpdatedActor : UpdatedActors)
    {
        FAIStimulus AIStimulus;
        AIStimulus = CanSenseActor(UpdatedActor, enemyAISenseCoilHead::Sight);
        if (AIStimulus.WasSuccessfullySensed())
        {
            HandleSensedSight(UpdatedActor);
        }
        AIStimulus = CanSenseActor(UpdatedActor, enemyAISenseCoilHead::Damage);
        if (AIStimulus.WasSuccessfullySensed())
        {
            HandleSensedDamage(UpdatedActor);
        }
    }*/
   
    for (AActor* UpdatedActor : UpdatedActors)
    {
        if (!UpdatedActor->ActorHasTag(TEXT("CoilHead")))
        {
            if (CanSenseActor(UpdatedActor, enemyAISenseCoilHead::Sight) || CanSenseActor(UpdatedActor, enemyAISenseCoilHead::Damage))
            {
                if (!isDead)
                {
                    GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Black, TEXT("hi"));
                    class ALSCoilHead* CoilHead = Cast<ALSCoilHead>(GetCharacter());
                    CoilHead->FindComponentByClass<ULSCoilHeadFSM>()->mState = ECoilHeadState::LookAtMe;
                    CoilHead->FindComponentByClass<ULSCoilHeadFSM>()->Anim->AnimState = ECoilHeadState::LookAtMe;
                }
            }
        }
    }

}

bool ALSCoilHeadController::GetActorPerception(AActor* Actor, FActorPerceptionBlueprintInfo& Info)
{
    bool bInfoFound = false;
    if (Actor != nullptr && Actor->IsPendingKillPending() == false)
    {
       // const FActorPerceptionInfo* PercetivedInfo = GetActorInfo(*Actor);
    }
    return bInfoFound;
}


bool ALSCoilHeadController::CanSenseActor(AActor* Actor, enemyAISenseCoilHead AIPerceptionSense)
{
    FActorPerceptionBlueprintInfo ActorPerceptionBlueprintInfo;
    FAIStimulus ResultStimulus;

    PerceptionComp->GetActorsPerception(Actor, ActorPerceptionBlueprintInfo);
    TSubclassOf<UAISense> QuerySenseClass;
    switch (AIPerceptionSense)
    {
    case enemyAISenseCoilHead::Sight:
        QuerySenseClass = UAISense_Sight::StaticClass();
        break;
    case enemyAISenseCoilHead::Damage:
        QuerySenseClass = UAISense_Damage::StaticClass();
        break;
    }

    TSubclassOf<UAISense> LastSensedStimulusClass;

    for (const FAIStimulus& AIStimulus : ActorPerceptionBlueprintInfo.LastSensedStimuli)
    {
        LastSensedStimulusClass = UAIPerceptionSystem::GetSenseClassForStimulus(this, AIStimulus);
        if (QuerySenseClass == LastSensedStimulusClass)
        {
            return true;
        }
    }
    return false;
}


bool ALSCoilHeadController::IsPlayerLookAtMe()
{
	if (!target)
	{
		UE_LOG(LogTemp, Warning, TEXT("CoilHead or Target is NULL!"));
		return false;
	}

	FActorPerceptionBlueprintInfo Info;
	PerceptionComp->GetActorsPerception(target, Info);

	for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
	{
		if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
		{
			if (Stimulus.WasSuccessfullySensed())
			{
				UE_LOG(LogTemp, Warning, TEXT("보고있다~~~"));
				return true;
			}
		}
	}

	return false;

}

void ALSCoilHeadController::MoveToRandomLocation()
{
    auto NavSys = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if (NavSys)
    {
        FNavLocation RandomLocation;
        if (NavSys->GetRandomReachablePointInRadius(me->GetActorLocation(), 500.0f, RandomLocation))
        {
            //CoilHead->MoveToLocation(RandomLocation.Location);
        }
    }
}

