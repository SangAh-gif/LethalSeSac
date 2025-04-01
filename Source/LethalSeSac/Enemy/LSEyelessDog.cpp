// Fill out your copyright notice in the Description page of Project Settings.


#include "LSEyelessDog.h"
#include "LSDogFSM.h"
#include "Components/BoxComponent.h"
#include "LSCharacter.h"

// Sets default values
ALSEyelessDog::ALSEyelessDog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FSM = CreateDefaultSubobject<ULSDogFSM>(TEXT("FSM"));

	ConstructorHelpers::FObjectFinder<USkeletalMesh> tempMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/SSA/Asset/EyelessDog/scene.scene'"));

	if (tempMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(tempMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(-40.0f, 0.0f, -92.0f), FRotator(0.0f, -90.0f, 0.0f));
		GetMesh()->SetRelativeScale3D(FVector(0.05f));
	}

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapBox->SetupAttachment(RootComponent);

	OverlapBox->SetBoxExtent(FVector(170.0f, 100.0f, 100.0f));
	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionObjectType(ECC_WorldDynamic);
	OverlapBox->SetCollisionResponseToChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECC_GameTraceChannel1, ECR_Overlap);

	// 
	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ALSEyelessDog::OnOverlapBegin);
	
}

// Called when the game starts or when spawned
void ALSEyelessDog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALSEyelessDog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ALSEyelessDog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALSEyelessDog::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ALSCharacter* Player = Cast<ALSCharacter>(OtherActor);
	{
		if (Player)
		{
			Player->Die();
		}
	}
}

