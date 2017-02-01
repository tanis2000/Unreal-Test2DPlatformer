// Fill out your copyright notice in the Description page of Project Settings.

#include "Test2DPlatformer.h"
#include "FollowerAIController.h"
#include "JumpingPathFollowingComponent.h"




AFollowerAIController::AFollowerAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UJumpingPathFollowingComponent>(TEXT("PathFollowingComponent")))
{

}