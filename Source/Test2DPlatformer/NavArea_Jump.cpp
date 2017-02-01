// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "NavArea_Jump.h"

UNavArea_Jump::UNavArea_Jump(const FObjectInitializer& ObjectInitializer)
        : Super(ObjectInitializer)
{
    FNavAreaHelper::Set(AreaFlags, ENavAreaFlag::Jump);
}


