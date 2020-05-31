// Fill out your copyright notice in the Description page of Project Settings.

#include "TestCharacter.h"
#include "Test2DPlatformer.h"
#include "PaperFlipbookComponent.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "Engine.h"
#include "Components/BoxComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
ATestCharacter::ATestCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    // Setup the assets
    struct FConstructorStatics
    {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
        FConstructorStatics()
        : IdleAnimationAsset(TEXT("/Game/Animations/HeroIdle.HeroIdle"))
        {
        }
    };
    static FConstructorStatics ConstructorStatics;
    
    IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();
    GetSprite()->SetFlipbook(IdleAnimation);
    
    // Use only Yaw from the controller and ignore the rest of the rotation.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    // Create an orthographic camera (no perspective) and attach it to the boom
    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 512.0f;
    SideViewCameraComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
    SideViewCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
    //SideViewCameraComponent->AttachTo(CameraBoom, USpringArmComponent::SocketName);
    
    // Prevent all automatic rotation behavior on the camera, character, and camera component
    SideViewCameraComponent->bUsePawnControlRotation = false;
    GetCharacterMovement()->bOrientRotationToMovement = false;

    // Configure character movement
    GetCharacterMovement()->GravityScale = 2.0f;
    GetCharacterMovement()->AirControl = 0.80f;
    GetCharacterMovement()->JumpZVelocity = 1000.f;
    GetCharacterMovement()->GroundFriction = 3.0f;
    GetCharacterMovement()->MaxWalkSpeed = 600.0f;
    GetCharacterMovement()->MaxFlySpeed = 600.0f;
    
    // Lock character motion onto the XZ plane, so the character can't move in or out of the screen
    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));
    
    // Behave like a traditional 2D platformer character, with a flat bottom instead of a curved capsule bottom
    // Note: This can cause a little floating when going up inclines; you can choose the tradeoff between better
    // behavior on the edge of a ledge versus inclines by setting this to true or false
    GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
    
    // Enable replication on the Sprite component so animations show up when networked
    GetSprite()->SetIsReplicated(true);
    bReplicates = true;

}

// Called when the game starts or when spawned
void ATestCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestCharacter::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ATestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
    PlayerInputComponent->BindAxis("MoveRight", this, &ATestCharacter::MoveRight);
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ATestCharacter::MoveRight(float Value)
{
    // Update animation to match the motion
    //UpdateAnimation();
    
    // Set the rotation so that the character faces his direction of travel.
    if (Controller != nullptr)
    {
        if (Value < 0.0f)
        {
            Controller->SetControlRotation(FRotator(0.0, 180.0f, 0.0f));
        }
        else if (Value > 0.0f)
        {
            Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
        }
    }
    
    // Apply the input to the character motion
    AddMovementInput(FVector(1.0f, 0.0f, 0.0f), Value);
}
