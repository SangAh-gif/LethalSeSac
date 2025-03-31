// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCharacter.h"
#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ItemBase.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LSGameModeBase.h"
#include "SpaceShipActor.h"
#include "LeverActor.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "DoorActor.h"
#include "MotionControllerComponent.h"
#include "Components/WidgetInteractionComponent.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Components/CapsuleComponent.h"

// Sets default values
ALSCharacter::ALSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	VRCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("VRCamera"));
	VRCamera->SetupAttachment(RootComponent);
	VRCamera->SetRelativeLocation(FVector(0,0,50));

	LeftHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHand->SetTrackingMotionSource(TEXT("Left"));
	LeftHand->SetupAttachment(RootComponent);

	RightHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHand->SetTrackingMotionSource(TEXT("Right"));
	RightHand->SetupAttachment(RootComponent);

	RightAim = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightAim"));
	RightAim->SetTrackingMotionSource(TEXT("RightAim"));
	RightAim->SetupAttachment(RootComponent);

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("scene"));
	scene->SetupAttachment(RightHand);

	WidgetInteractionComp = CreateDefaultSubobject<UWidgetInteractionComponent>(TEXT("WidgetInteractionComp"));
	WidgetInteractionComp->SetupAttachment(RightAim);

	// Crouch 설정켜기
	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;

	ConstructorHelpers::FObjectFinder<UInputMappingContext> TempIMC (TEXT("/Script/EnhancedInput.InputMappingContext'/Game/KHH/Input/IMC_LS.IMC_LS'"));
	if (TempIMC.Succeeded())
	{
		IMC_LS = TempIMC.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Move(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSMove.IA_LSMove'"));
	if (TempIA_Move.Succeeded())
	{
		IA_Move = TempIA_Move.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Turn(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSTurn.IA_LSTurn'"));
	if (TempIA_Turn.Succeeded())
	{
		IA_Turn = TempIA_Turn.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Run(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSRun.IA_LSRun'"));
	if (TempIA_Run.Succeeded())
	{
		IA_Run = TempIA_Run.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Duck(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSDuck.IA_LSDuck'"));
	if (TempIA_Duck.Succeeded())
	{
		IA_Duck = TempIA_Duck.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Interact(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSInteract.IA_LSInteract'"));
	if (TempIA_Interact.Succeeded())
	{
		IA_Interact = TempIA_Interact.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Jump(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSJump.IA_LSJump'"));
	if (TempIA_Jump.Succeeded())
	{
		IA_Jump = TempIA_Jump.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Throw(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSThrow.IA_LSThrow'"));
	if (TempIA_Throw.Succeeded())
	{
		IA_Throw = TempIA_Throw.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_ChangeItem(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSChangeItem.IA_LSChangeItem'"));
	if (TempIA_ChangeItem.Succeeded())
	{
		IA_ChangeItem = TempIA_ChangeItem.Object;
	}

	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Use(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSUse.IA_LSUse'"));
	if (TempIA_Use.Succeeded())
	{
		IA_UseItem = TempIA_Use.Object;
	}
	ConstructorHelpers::FObjectFinder<UInputAction> TempIA_Scan(TEXT("/Script/EnhancedInput.InputAction'/Game/KHH/Input/IA_LSScan.IA_LSScan'"));
	if (TempIA_Scan.Succeeded())
	{
		IA_Scan = TempIA_Scan.Object;
	}
}

// Called when the game starts or when spawned
void ALSCharacter::BeginPlay()
{
	Super::BeginPlay();
	ItemArray.SetNum(4);
	Super::BeginPlay();
	GM = Cast<ALSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));

	if (UHeadMountedDisplayFunctionLibrary::IsHeadMountedDisplayEnabled())
	{
		UHeadMountedDisplayFunctionLibrary::SetTrackingOrigin(EHMDTrackingOrigin::View);
		//UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition(180);
	}
}

// Called every frame
void ALSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (!bIsCrouched)
	{
		if (bIsMoving)
		{
			CurSoundTime += DeltaTime;
			if (bRun)
			{
				loud = Runloud;
				if(CurSoundTime >= RunSoundTime) WalkSound(loud);
			}
			else
			{
				loud = Walkloud;
				if(CurSoundTime >= WalkSoundTime) WalkSound(loud);
			}
		}
	}
}

// Called to bind functionality to input
void ALSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	APlayerController* PC = Cast<APlayerController>(Controller);
	if (PC)
	{
		if(UEnhancedInputLocalPlayerSubsystem* ss = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			ss->AddMappingContext(IMC_LS, 1);
			ss->AddMappingContext(IMC_Hand, 1);
		}
	}

	UEnhancedInputComponent* InputSys = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (InputSys)
	{
		InputSys->BindAction(IA_Move, ETriggerEvent::Triggered, this, &ALSCharacter::Move);
		InputSys->BindAction(IA_Move, ETriggerEvent::Started, this, &ALSCharacter::ChangeMove);
		InputSys->BindAction(IA_Move, ETriggerEvent::Completed, this, &ALSCharacter::ChangeMove);
		InputSys->BindAction(IA_Turn, ETriggerEvent::Triggered, this, &ALSCharacter::Turn);
		InputSys->BindAction(IA_Run, ETriggerEvent::Started, this, &ALSCharacter::RunStart);
		InputSys->BindAction(IA_Run, ETriggerEvent::Completed, this, &ALSCharacter::RunEnd);
		InputSys->BindAction(IA_Duck, ETriggerEvent::Started, this, &ALSCharacter::DuckStart);
		InputSys->BindAction(IA_Duck, ETriggerEvent::Completed, this, &ALSCharacter::DuckEnd);
		InputSys->BindAction(IA_Jump, ETriggerEvent::Started, this, &ALSCharacter::Jump);
		InputSys->BindAction(IA_Jump, ETriggerEvent::Completed, this, &ALSCharacter::StopJumping);
		InputSys->BindAction(IA_Interact, ETriggerEvent::Triggered, this, &ALSCharacter::Interact);
		InputSys->BindAction(IA_Throw, ETriggerEvent::Started, this, &ALSCharacter::Throw);
		InputSys->BindAction(IA_ChangeItem, ETriggerEvent::Triggered, this, &ALSCharacter::ChangeItem);
		InputSys->BindAction(IA_UseItem, ETriggerEvent::Started, this, &ALSCharacter::Use);
		InputSys->BindAction(IA_UseItem, ETriggerEvent::Completed, this, &ALSCharacter::ReleaseUI);
		InputSys->BindAction(IA_Scan, ETriggerEvent::Started, this, &ALSCharacter::Scan);
	}
}

void ALSCharacter::Move(const struct FInputActionValue& val)
{
	FVector2D scale = val.Get<FVector2D>();
	FVector Dir = VRCamera->GetForwardVector() * scale.X + VRCamera->GetRightVector() * scale.Y;
	AddMovementInput(Dir);
}

void ALSCharacter::ChangeMove()
{
	bIsMoving = !bIsMoving;
}

void ALSCharacter::Turn(const struct FInputActionValue& val)
{
	if (bUsingMouse == false)
	{
		return;
	}
	FVector2D scale = val.Get<FVector2D>();
	AddControllerPitchInput(scale.Y);
	AddControllerYawInput(scale.X);
}

void ALSCharacter::RunStart()
{
	GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	bRun = true;
}

void ALSCharacter::RunEnd()
{
	GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	bRun = false;
}

void ALSCharacter::DuckStart()
{
	ALSCharacter::Crouch();
}

void ALSCharacter::DuckEnd()
{
	ALSCharacter::UnCrouch();
}

void ALSCharacter::Interact()
{
	TArray<FHitResult> HitInfos;
	bool bHit = drawInteractLine(HitInfos);
	if (bHit)
	{
		for (FHitResult HitInfo : HitInfos)
		{
			AItemBase* item = Cast<AItemBase>(HitInfo.GetActor());
			if (item)
			{
				if (!ItemArray[SelectedIndex])
				{
					ItemArray[SelectedIndex] = item;
					item->BoxComp->SetSimulatePhysics(false);
					//item->BoxComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					item->MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					item->AttachToComponent(scene, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
				}
				break;
			}
			ALeverActor* lever = Cast<ALeverActor>(HitInfo.GetActor());
			if (lever)
			{
				lever->InteractLever();
				break;
			}

			ADoorActor* door = Cast<ADoorActor>(HitInfo.GetActor());
			if (door)
			{
				door->OpenDoor();
				break;
			}
		}
			
	}
}

void ALSCharacter::Throw()
{
	if(!ItemArray[SelectedIndex]) return;
	ItemArray[SelectedIndex]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	//ItemArray[SelectedIndex]->BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemArray[SelectedIndex]->BoxComp->SetSimulatePhysics(true);
	ItemArray[SelectedIndex]->MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	ItemArray[SelectedIndex] = nullptr;
	MakeNoise(2, this, GetActorLocation(), 1000, TEXT("Noise"));
}

void ALSCharacter::ChangeItem(const struct FInputActionValue& val)
{
	int32 preIndex = SelectedIndex;
	SelectedIndex += (int32)val.Get<float>();
	SelectedIndex = SelectedIndex < 0 ? 3 : SelectedIndex;
	SelectedIndex %= 4;
	UE_LOG(LogTemp, Warning, TEXT("%d"), SelectedIndex);
	SelectItem(SelectedIndex, preIndex);
}

void ALSCharacter::SelectItem(int32 index, int32 preIndex)
{
	if (ItemArray[preIndex])
	{
		ItemArray[preIndex]->BoxComp->SetVisibility(false);
		ItemArray[preIndex]->SetActorHiddenInGame(true);
	}
	if(ItemArray[index])
	{
		ItemArray[index]->BoxComp->SetVisibility(true);
		ItemArray[index]->SetActorHiddenInGame(false);
	}
}

void ALSCharacter::Use()
{
	if (WidgetInteractionComp)
	{
		WidgetInteractionComp->PressPointerKey(EKeys::LeftMouseButton);
	}

	if (ItemArray[SelectedIndex])
	{
		ItemArray[SelectedIndex]->UseItem();
	}
	
}

void ALSCharacter::ReleaseUI()
{
	WidgetInteractionComp->ReleasePointerKey(EKeys::LeftMouseButton);
}



bool ALSCharacter::drawInteractLine(TArray<FHitResult>& HitInfos)
{
	FVector StartPos = RightAim->GetComponentLocation();
	FVector EndPos = StartPos + RightAim->GetForwardVector() * InteractDist;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	for (int i = 0; i < 4; ++i)
	{
		if(ItemArray[i])
		params.AddIgnoredActor(ItemArray[i]);
	}
	bool bHit = GetWorld()->SweepMultiByChannel(HitInfos, StartPos, EndPos, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(50), params);

	return bHit;
}

void ALSCharacter::Die()
{
	bIsDead = true;
	ASpaceShipActor* SpaceShip = Cast<ASpaceShipActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpaceShipActor::StaticClass()));
	APlayerController* pc = Cast<APlayerController>(Controller);
	pc->SetViewTarget(SpaceShip);
	SpaceShip->AfterEnd();
	/*if (pc)
	{
		pc->SetPause(true);
		pc->bShowMouseCursor = true;
	}*/
}

void ALSCharacter::WalkSound(float loudness)
{
	MakeNoise(loudness, this, GetActorLocation());
	//UE_LOG(LogTemp,Warning,TEXT("WALKSOUND!"));
	CurSoundTime = 0.0f;
}

void ALSCharacter::Scan()
{
	TArray<AActor*> postArr;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), APostProcessVolume::StaticClass(), TEXT("Scan"), postArr);
	for (auto post : postArr)
	{
		APostProcessVolume* scanPost = Cast<APostProcessVolume>(post);
		if (scanPost)
		{
			scanPost->bUnbound = true;
		}
		GetWorld()->GetTimerManager().SetTimer(ScanTimer, FTimerDelegate::CreateLambda(
			[this, scanPost]()
			{
				CurScanTime += GetWorld()->DeltaTimeSeconds;
				if (CurScanTime >= ScanTime)
				{
					scanPost->bUnbound = false;
					GetWorldTimerManager().ClearTimer(ScanTimer);
					CurScanTime = 0;
				}
			}), 0.02f, true);
	}
	TArray<FHitResult> HitInfos;
	TArray<AItemBase*> HitItems;
	FVector CurPos = GetActorLocation();
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	for (int i = 0; i < ItemArray.Num(); ++i)
	{
		if (ItemArray[i])
			params.AddIgnoredActor(ItemArray[i]);
	}
	bool bHit = GetWorld()->SweepMultiByChannel(HitInfos, CurPos, CurPos, FQuat::Identity, ECC_Visibility, FCollisionShape::MakeSphere(ScanDist),params);
	if (bHit)
	{
		for (auto hitinfo : HitInfos)
		{
			AItemBase* item = Cast<AItemBase>(hitinfo.GetActor());
			if (item)
			{
				HitItems.Emplace(item);
			}
		}
		for (auto hititem : HitItems)
		{
			hititem->ShowInfo();
		}
	}
}

void ALSCharacter::EndGame()
{
	GM->GameDay --;
	GM->bInSpace = true;
	if (bIsDead)
	{
		GM->TotValue = 0;
	}
	else
	{
		ASpaceShipActor* ss = Cast<ASpaceShipActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASpaceShipActor::StaticClass()));
		GM->TotValue = ss->totVal;
	}
	GM->SaveGameData();

	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StartMap"));
}

void ALSCharacter::WinGame()
{
	if (!bSucceed)
	{
		GetCapsuleComponent()->SetSimulatePhysics(true);
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		GetCapsuleComponent()->AddImpulse(GetActorRightVector() * -500000);

	}
	GetWorld()->GetTimerManager().SetTimer(EndingTimer, FTimerDelegate::CreateLambda(
		[this]()
		{
			CurEndingTime += GetWorld()->DeltaTimeSeconds;
			if (CurScanTime >= EndingTime)
			{
				UGameplayStatics::SetGamePaused(GetWorld(), true);
				GetWorldTimerManager().ClearTimer(EndingTimer);
				CurEndingTime = 0;
			}
		}), 0.02f, true);

}
