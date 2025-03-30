// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCoilHeadController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LSCoilHead.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "LSCoilHeadFSM.h"
#include "NavigationSystem.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"

ALSCoilHeadController::ALSCoilHeadController()
{
    PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
    SetPerceptionComponent(*PerceptionComp);

    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
    SightConfig->SetMaxAge(3.0f);

    if (SightConfig)
    {
        SightConfig->SightRadius                  = 1000.0f;
        SightConfig->LoseSightRadius              = 1200.0f;
        SightConfig->PeripheralVisionAngleDegrees = 180.0f;

        PerceptionComp->ConfigureSense(*SightConfig);
        PerceptionComp->SetDominantSense(SightConfig->GetSenseImplementation());
    }
   
  
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
    if(!PerceptionComp || !coilHeadFSM) return;

    for (AActor* UpdatedActor : UpdatedActors)
    {
        if (!UpdatedActor->ActorHasTag("enemy"))
        {
            FActorPerceptionBlueprintInfo Info;
            PerceptionComp->GetActorsPerception(UpdatedActor, Info);

            for (const FAIStimulus& Stimulus : Info.LastSensedStimuli)
            {
                if (UAIPerceptionSystem::GetSenseClassForStimulus(this, Stimulus) == UAISense_Sight::StaticClass())
                {
                    /*if (Stimulus.WasSuccessfullySensed())
                    {
                        coilHeadFSM->mState = ECoilHeadState::LookAtMe;
                    }
                    else
                    {
                        coilHeadFSM->mState = ECoilHeadState::Patrol;
                    }*/
                    ALSCoilHead* enemy = Cast<ALSCoilHead>(GetCharacter());
                    if (enemy)
                    {
                        ULSCoilHeadFSM* FSM = enemy->FindComponentByClass<ULSCoilHeadFSM>();
                        if (FSM)
                        {
                            FSM->LookAtMeState();
                        }
                    }
                }
            }
        }
    }
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
        if (NavSys->GetRandomReachablePointInRadius(CoilHead->GetActorLocation(), 500.0f, RandomLocation))
        {
            //CoilHead->MoveToLocation(RandomLocation.Location);
        }
    }
}

