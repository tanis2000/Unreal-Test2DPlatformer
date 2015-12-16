// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "Mob.generated.h"

UCLASS()
class TEST2DPLATFORMER_API AMob : public AActor
{
	GENERATED_BODY()
	
    // Name of the Sprite component
    static FName SpriteComponentName;

public:
	// Sets default values for this actor's properties
	AMob();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

    void MoveRight(float Value);

    UFUNCTION()
    void OnBeginOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
    UFUNCTION()
    void OnEndOverlap(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    class UInputComponent *InputComponent;

    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPaperSpriteComponent* SpriteComponent;
    
    class UPaperSprite* MainSprite;

};
