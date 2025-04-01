// Fill out your copyright notice in the Description page of Project Settings.


#include "LSEnemyManager.h"
#include "LSCoilHead.h"
#include "LSEyelessDog.h"
#include "NavigationSystem.h"

// Sets default values
ALSEnemyManager::ALSEnemyManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FClassFinder<ALSCoilHead> CoilHeadFinder(TEXT("/Game/Path/To/CoilHead_BP.CoilHead_BP_C"));
    if (CoilHeadFinder.Succeeded())
    {
        CoilHeadClass = CoilHeadFinder.Class;
    }

    static ConstructorHelpers::FClassFinder<ALSEyelessDog> EyelessDogFinder(TEXT("/Game/Path/To/EyelessDog_BP.EyelessDog_BP_C"));
    if (EyelessDogFinder.Succeeded())
    {
        EyelessDogClass = EyelessDogFinder.Class;
    }

}

// Called when the game starts or when spawned
void ALSEnemyManager::BeginPlay()
{
	Super::BeginPlay();
	
    SpawnMonsters();
}

// Called every frame
void ALSEnemyManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool ALSEnemyManager::GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& OutLocation)
{
    auto NavigationSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());
    if (!NavigationSystem)
    {
        UE_LOG(LogTemp, Warning, TEXT("Navigation System not found"));
        return false;
    }

    FNavLocation NavLocation;
    bool bSuccess = NavigationSystem->GetRandomReachablePointInRadius(CenterLocation, Radius, NavLocation);
    if (bSuccess)
    {
        OutLocation = NavLocation.Location;
    }
    return bSuccess;
}

void ALSEnemyManager::SpawnMonsters()
{
    //if (!bCoilHeadSpawned && CoilHeadClass)
    //{
    //    FVector SpawnLocation;
    //    if (GetRandomPositionInNavMesh(ExteriorCenter, ExteriorRadius, SpawnLocation))
    //    {
    //        FActorSpawnParameters SpawnParams;
    //        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingasc = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    //        SpawnParams.Owner = this; // EnemyManager가 소유자로 설정
    //        ALSCoilHead* CoilHead = GetWorld()->SpawnActor<ALSCoilHead>(CoilHeadClass, SpawnLocation, FRotator(0.0f), SpawnParams);
    //        if (CoilHead)
    //        {
    //            bCoilHeadSpawned = true;
    //            UE_LOG(LogTemp, Warning, TEXT("CoilHead spawned at: %s"), *SpawnLocation.ToString());
    //        }
    //    }
    //}

    if (!bEyelessDogSpawned && EyelessDogClass)
    {
        FVector SpawnLocation;
        if (GetRandomPositionInNavMesh(InteriorCenter, InteriorRadius, SpawnLocation))
        {
            FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
            SpawnParams.Owner = this;
            ALSEyelessDog* EyelessDog = GetWorld()->SpawnActor<ALSEyelessDog>(EyelessDogClass, SpawnLocation, FRotator(0.0f), SpawnParams);
            if (EyelessDog)
            {
                bEyelessDogSpawned = true;
                UE_LOG(LogTemp, Warning, TEXT("EyelessDog spawned at: %s"), *SpawnLocation.ToString());
            }
        }
    }
}

