// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpaceShipActor.generated.h"

UCLASS()
class LETHALSESAC_API ASpaceShipActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceShipActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	class UBoxComponent* CheckItemComp;

	UPROPERTY(EditAnywhere)
	class USceneComponent* RootComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 totVal = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Quota = 200;

	FTimerHandle EndingTimer;

	void AfterEnd();

	float CurTime = 0;
	float boardTime = 7;

	UFUNCTION()
	void OnSpaceShipOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,	bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnSpaceShipEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
