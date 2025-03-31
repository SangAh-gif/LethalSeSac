// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "LSCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkyLight.h"
#include "Components/SkyLightComponent.h"
#include "Components/BoxComponent.h"

// Sets default values
ADoorActor::ADoorActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	SetRootComponent(BoxComp);
}

// Called when the game starts or when spawned
void ADoorActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADoorActor::OpenDoor()
{
	auto player = Cast<ALSCharacter>(UGameplayStatics::GetPlayerPawn(GetWorld(),0));
	if (player)
	{
		ASkyLight* skyLight = Cast<ASkyLight>(UGameplayStatics::GetActorOfClass(GetWorld(), ASkyLight::StaticClass()));
		if (bIsOutSide) // 밖이면
		{
			player->SetActorLocation(FVector(1480,15870, -2200)); // 안으로
			skyLight->GetLightComponent()->SetVisibility(false);
		}
		else // 안이면
		{
			player->SetActorLocationAndRotation(FVector(400, -4985, 155),FRotator(0, 0, -180)); // 밖으로
			skyLight->GetLightComponent()->SetVisibility(true);
		}
	}
}
