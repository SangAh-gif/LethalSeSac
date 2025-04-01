// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCoilHead.h"
#include "Components/BoxComponent.h"
#include "LSCharacter.h"

// Sets default values
ALSCoilHead::ALSCoilHead()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FSM = CreateDefaultSubobject<ULSCoilHeadFSM>(TEXT("FSM"));
	Tags.Add(FName("CoilHead"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SSA/Asset/CoilHead/SKM_Untitled.SKM_Untitled'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -84.0f), FRotator(0.0f, -90.0f, 0.0f));
	}	

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);

	OverlapBox->SetBoxExtent(FVector(40.0f, 40.0f, 70.0f));
	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionObjectType(ECC_WorldDynamic);
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALSCoilHead::OnOverlapBegin);
}

// Called when the game starts or when spawned
void ALSCoilHead::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALSCoilHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALSCoilHead::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALSCoilHead::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALSCharacter* Player = Cast<ALSCharacter>(OtherActor);
	{
		if (Player)
		{
			Player->Die();
		}
	}
}

