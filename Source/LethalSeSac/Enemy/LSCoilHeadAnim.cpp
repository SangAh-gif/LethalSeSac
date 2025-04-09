// Fill out your copyright notice in the Description page of Project Settings.


#include "LSCoilHeadAnim.h"
#include "LSCoilHead.h"

ULSCoilHeadAnim::ULSCoilHeadAnim()
{

}

void ULSCoilHeadAnim::AnimNotify_OnSound()
{
	if (ALSCoilHead* CoilHead = Cast<ALSCoilHead>(TryGetPawnOwner()))
	{
		CoilHead->FSM->OnSound();
	}
}
