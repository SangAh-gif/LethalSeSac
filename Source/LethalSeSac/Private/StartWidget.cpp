// Fill out your copyright notice in the Description page of Project Settings.


#include "StartWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UStartWidget::QuitVRGame()
{
	auto pc = GetWorld()->GetFirstPlayerController();
#if WITH_EDITOR
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, false);
#else
	UKismetSystemLibrary::QuitGame(GetWorld(), pc, EQuitPreference::Quit, true);
#endif

}
