// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LSEnemyManager.generated.h"

UCLASS()
class LETHALSESAC_API ALSEnemyManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALSEnemyManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
    // 몬스터 클래스
    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<ALSCoilHead> CoilHeadClass;

    UPROPERTY(EditAnywhere, Category = "Spawning")
    TSubclassOf<ALSEyelessDog> EyelessDogClass;

    // 스폰 여부 플래그
    bool bCoilHeadSpawned = false;
    bool bEyelessDogSpawned = false;

	UPROPERTY(EditAnywhere, Category = "Spawning")
    FVector ExteriorCenter = FVector(0.0f, 0.0f, 0.0f); // 외부 중심

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float ExteriorRadius = 1000.0f; // 외부 반경

    UPROPERTY(EditAnywhere, Category = "Spawning")
    FVector InteriorCenter = FVector(500.0f, 500.0f, 0.0f); // 내부 중심

    UPROPERTY(EditAnywhere, Category = "Spawning")
    float InteriorRadius = 500.0f; // 내부 반경

    // 랜덤 위치 계산 함수
    bool GetRandomPositionInNavMesh(FVector CenterLocation, float Radius, FVector& OutLocation);

    // 스폰 함수
    void SpawnMonsters();
	
};
