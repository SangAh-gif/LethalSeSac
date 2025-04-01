// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LSCharacter.generated.h"

UCLASS()
class LETHALSESAC_API ALSCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// 카메라
	UPROPERTY(EditAnywhere)
	class UCameraComponent* VRCamera;
	UPROPERTY(EditAnywhere)
	class USceneComponent* scene;
	UPROPERTY(EditAnywhere, Category = "MotionController")
	class UMotionControllerComponent* LeftHand;
	UPROPERTY(EditAnywhere, Category = "MotionController")
	class UMotionControllerComponent* RightHand;
	UPROPERTY(EditAnywhere, Category = "MotionController")
	class UMotionControllerComponent* RightAim;

	UPROPERTY(VisibleAnywhere, Category = "Widget")
	class UWidgetInteractionComponent* WidgetInteractionComp;


public: // 이동 및 회전
	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC_LS;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	class UInputMappingContext* IMC_Hand;

	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Turn;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Run;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Duck;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Interact;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Throw;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_ChangeItem;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_UseItem;
	UPROPERTY(EditAnywhere, Category = Input)
	class UInputAction* IA_Scan;

	void Move(const struct FInputActionValue& val);
	void ChangeMove();
	void Turn(const struct FInputActionValue& val);
	void RunStart();
	void RunEnd();
	void DuckStart();
	void DuckEnd();
	void Interact();
	void Throw();
	void ChangeItem(const struct FInputActionValue& val);
	void SelectItem(int32 index, int32 preIndex);
	void Use();
	void ReleaseUI();
	bool drawInteractLine(TArray<FHitResult>& HitInfos);
	void Die();
	void WalkSound(float loud);
	void Scan();
	void EndGame();
	void WinGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSpeed = 1000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSpeed = 600;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InteractDist = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanDist = 1500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WalkSoundTime = 0.6f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RunSoundTime = 0.3f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurSoundTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurScanTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ScanTime = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Input")
	bool bUsingMouse = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bRun = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Walkloud =1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Runloud =2;
	float loud = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SelectedIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Sta = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurEndingTime = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EndingTime = 2.0f;
	bool bIsMoving = false;
	bool bSucceed = false;

	FTimerHandle ScanTimer;
	FTimerHandle EndingTimer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AItemBase*> ItemArray;

	class ALSGameModeBase* GM;

	bool bIsDead = false;


};
