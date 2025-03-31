// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverActor.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "LSGameModeBase.h"
#include "LSCharacter.h"

// Sets default values
ALeverActor::ALeverActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetBoxExtent(FVector(50));
	BoxComp->SetRelativeLocation(FVector(-130, 9010, 1225));

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);

	ConstructorHelpers::FObjectFinder<UStaticMesh> TempMesh(TEXT("/Script/Engine.StaticMesh'/Game/SSA/Ship/ShipMesh/Lever_001_Baked1.Lever_001_Baked1'"));
	if (TempMesh.Succeeded())
	{
		MeshComp->SetStaticMesh(TempMesh.Object);
	}
}

// Called when the game starts or when spawned
void ALeverActor::BeginPlay()
{
	Super::BeginPlay();
	GM = Cast<ALSGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
}

// Called every frame
void ALeverActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverActor::InteractLever()
{
	GM->bInSpace = !bInSpace;
	if (bInSpace) // 우주라면
	{
		GM->SaveGameData();
		UGameplayStatics::OpenLevel(GetWorld(),TEXT("level1"));
	}
	else
	{
		ALSCharacter* player = Cast<ALSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(), 0));
		player->EndGame();
	}
}

