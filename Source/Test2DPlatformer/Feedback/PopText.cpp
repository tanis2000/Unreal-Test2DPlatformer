// Fill out your copyright notice in the Description page of Project Settings.


#include "PopText.h"

#include "PopTextWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APopText::APopText()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	RootComponent = Billboard;

	Duration = 1.5f;
	ZFinalOffset = 10;
}

// Called when the game starts or when spawned
void APopText::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Display, TEXT("Attempting to create widget"));
	CurrentWidget = CreateWidget<UPopTextWidget>(GetWorld(), StartingWidgetClass);
	CurrentWidget->AddToViewport();

	CurrentDuration = Duration;
	CurrentZOffset = 0;
}

// Called every frame
void APopText::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UpdatePosition(DeltaTime);
}

void APopText::UpdatePosition(float DeltaTime)
{
	if (CurrentWidget == nullptr)
	{
		return;
	}
	
	CurrentDuration -= DeltaTime;
	if (CurrentDuration <= 0)
	{
		CurrentWidget->RemoveFromParent();
		CurrentWidget = nullptr;
		return;
	}

	FVector WorldLocation = GetActorLocation();
	const float Ratio = (Duration - CurrentDuration) / Duration;
	CurrentZOffset = ZFinalOffset * Ratio;
	WorldLocation.Z += CurrentZOffset;
	
	FVector2d ScreenPosition;
	UGameplayStatics::ProjectWorldToScreen(GetWorld()->GetFirstPlayerController(), WorldLocation, ScreenPosition);
	CurrentWidget->SetPositionInViewport(ScreenPosition);
}

void APopText::SetContent(FString Content)
{
	if (CurrentWidget == nullptr)
	{
		return;
	}
	CurrentWidget->SetContent(Content);
}

// void APopText::ChangeWidget(TSubclassOf<UUserWidget> NewWidgetClass)
// {
// 	if (CurrentWidget != nullptr)
// 	{
// 		CurrentWidget->RemoveFromViewport();
// 		CurrentWidget = nullptr;
// 	}
// 	if (NewWidgetClass != nullptr)
// 	{
// 		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), NewWidgetClass);
// 		if (CurrentWidget != nullptr)
// 		{
// 			CurrentWidget->AddToViewport();
// 		}
// 	}
// }