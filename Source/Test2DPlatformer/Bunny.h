// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "Bunny.generated.h"

UCLASS()
class TEST2DPLATFORMER_API ABunny : public APawn
{
	GENERATED_BODY()

    // Name of the Sprite component
    static FName SpriteComponentName;
private:
    
public:
	// Sets default values for this pawn's properties
	ABunny();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  virtual void ReceiveHit(class UPrimitiveComponent *MyComp, AActor *Other, class UPrimitiveComponent *OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult &Hit);
  
  void OnMyActorHit(AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

  
    /** The main skeletal mesh associated with this Character (optional sub-object). */
    UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
    class UPaperSpriteComponent* Sprite;

    class UPaperSprite* IdleSprite;
  class UPaperSpriteAtlas *IdleSpriteAtlas;

    float posX = 0;
    float posY = 0;
    float speedX = 0;
    float speedY = 0;
    float gravity = 0.5f;
    float maxX = 300;
    float maxY = 300;
    float minX = 0;
    float minY = 0;
    FVector newPos = FVector();

	
};
