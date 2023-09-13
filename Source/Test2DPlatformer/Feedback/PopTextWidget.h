// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PopTextWidget.generated.h"

/**
 * 
 */
UCLASS()
class TEST2DPLATFORMER_API UPopTextWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintImplementableEvent, Category="Something")
	void SetContent(const FString &Content);

};
