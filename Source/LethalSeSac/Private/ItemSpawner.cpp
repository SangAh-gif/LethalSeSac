// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemSpawner.h"
#include "ItemBase.h"

// Sets default values
AItemSpawner::AItemSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("scene"));
	SetRootComponent(scene);

}

// Called when the game starts or when spawned
void AItemSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	int32 ItemIdx = FMath::RandRange(1, ItemList.Num())-1;
	if(!ItemList[ItemIdx]) return;
	
	GetWorld()->SpawnActor<AItemBase>(ItemList[ItemIdx], GetActorLocation(), GetActorRotation());
}

// Called every frame
void AItemSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

