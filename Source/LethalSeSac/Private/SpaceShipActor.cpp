// Fill out your copyright notice in the Description page of Project Settings.


#include "SpaceShipActor.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "ItemBase.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LeverActor.h"
#include "GameFramework/CharacterMovementComponent.h"


// Sets default values
ASpaceShipActor::ASpaceShipActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	CheckItemComp = CreateDefaultSubobject<UBoxComponent>(TEXT("CheckItemComp"));
	CheckItemComp->SetupAttachment(RootComponent);
	CheckItemComp->SetRelativeLocation(FVector(15.000000, 730.000000, 275.000000));
	CheckItemComp->SetRelativeScale3D(FVector(9.500000, 29.000000, 7.000000));
	CheckItemComp->SetCollisionProfileName(TEXT("itemcheck"));

	CheckItemComp->OnComponentBeginOverlap.AddDynamic(this, &ASpaceShipActor::OnSpaceShipOverlap);
	CheckItemComp->OnComponentEndOverlap.AddDynamic(this, &ASpaceShipActor::OnSpaceShipEndOverlap);
}

// Called when the game starts or when spawned
void ASpaceShipActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpaceShipActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceShipActor::AfterEnd()
{
	ALSCharacter* player = Cast<ALSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	ALeverActor* Lever = Cast<ALeverActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ALeverActor::StaticClass()));
	player->AttachToComponent(CheckItemComp, FAttachmentTransformRules::KeepWorldTransform);
	//player->GetCharacterMovement()->DisableMovement();
	Lever->AttachToComponent(RootComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
			FVector CurPos = GetActorLocation();
	GetWorld()->GetTimerManager().SetTimer(EndingTimer, FTimerDelegate::CreateLambda([this,CurPos]()
		{
			FVector EndPos = FVector(-22720, -7250, 7800);
			CurTime += 0.02;
			FVector NCurPos = FMath::Lerp(CurPos, EndPos, FMath::Pow(CurTime/boardTime,3));
			SetActorLocation(NCurPos,true);
			if (CurTime >= boardTime)
			{
				SetActorLocation(EndPos);
				GetWorldTimerManager().ClearTimer(EndingTimer);
				ALSCharacter * player = Cast<ALSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
				if (player)
				{
					player->EndGame();
				}
			}
		}), 0.02,true);


	
}

void ASpaceShipActor::OnSpaceShipOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AItemBase* item = Cast<AItemBase>(OtherActor);
	if (item)
	{
		UE_LOG(LogTemp, Warning, TEXT("ITEM IN!!!!!!!!!!!!"));
		totVal += item->curVal;
	}
}

void ASpaceShipActor::OnSpaceShipEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AItemBase* item = Cast<AItemBase>(OtherActor);
	if (item)
	{
		UE_LOG(LogTemp, Warning, TEXT("ITEM OUT!!!!!!!!!!!!"));
		totVal -= item->curVal;
	}
}

