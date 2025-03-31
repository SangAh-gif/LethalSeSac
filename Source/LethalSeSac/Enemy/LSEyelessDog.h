// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LSEyelessDog.generated.h"

UCLASS()
class LETHALSESAC_API ALSEyelessDog : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALSEyelessDog();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	class ULSDogFSM* FSM;

	bool Damaged = true; 

public:
	UPROPERTY(VisibleAnywhere, Category = "Collision")
	class UBoxComponent* OverlapBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,
						AActor* OtherActor,
						UPrimitiveComponent* OtherComp,
						int32 OtherBodyIndex,
						bool bFromSweep,
						const FHitResult& SweepResult);
};
