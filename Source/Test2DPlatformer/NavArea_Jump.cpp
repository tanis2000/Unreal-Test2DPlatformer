// Fill out your copyright notice in the Description page of Project Settings.

#include "NavArea_Jump.h"
#include "Test2DPlatformer.h"

/*
UNavArea_Jump::UNavArea_Jump(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
{
    FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);
}
*/

UNavArea_Jump::UNavArea_Jump()
{
	FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);
}
