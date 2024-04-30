// Fill out your copyright notice in the Description page of Project Settings.

#include "HeroPawn.h"
#include "Test2DPlatformer.h"
#include "PaperFlipbookComponent.h"
#include "BunnyManager.h"
#include "PixelPerfectCameraComponent.h"
#include "Particles/ParticleSystem.h"
#include "PaperFlipbook.h"
#include "PaperSprite.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/BoxComponent.h"
#include "Components/InputComponent.h"
#include "Kismet/GameplayStatics.h"
#include "EngineUtils.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/LocalPlayer.h"
#include "FAPPlayerController.h"
#include "Ball.h"
#include "Feedback/PopText.h"
#include "Weapon/Bullet.h"

FName AHeroPawn::SpriteComponentName(TEXT("Sprite0"));


// Sets default values
AHeroPawn::AHeroPawn(const FObjectInitializer& ObjectInitializer): Super(ObjectInitializer) {
    // Setup the assets
    struct FConstructorStatics {
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> IdleAnimationAsset;
        ConstructorHelpers::FObjectFinderOptional<UPaperFlipbook> Player2IdleAnimationAsset;

        FConstructorStatics()
                : 
                IdleAnimationAsset(TEXT("/Game/Animations/HeroIdle.HeroIdle")),
                Player2IdleAnimationAsset(TEXT("/Game/Animations/HeroBlueIdle.HeroBlueIdle"))
                 {
        }
    };
    static FConstructorStatics ConstructorStatics;

    IdleAnimation = ConstructorStatics.IdleAnimationAsset.Get();

    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    // Create a dummy root component we can attach things to.
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
    UE_LOG(LogTemp, Warning, TEXT("TEST"));

    RunAnimation = CreateDefaultSubobject<UPaperFlipbook>("RunAnimation");
    JumpAnimation = CreateDefaultSubobject<UPaperFlipbook>("JumpAnimation");
    FallAnimation = CreateDefaultSubobject<UPaperFlipbook>("FallAnimation");
    WallGrabAnimation = CreateDefaultSubobject<UPaperFlipbook>("WallGrabAnimation");
    ShootAnimation = CreateDefaultSubobject<UPaperFlipbook>("ShootAnimation");

    // Try to create the sprite component
    Sprite = CreateOptionalDefaultSubobject<UPaperFlipbookComponent>(AHeroPawn::SpriteComponentName);
    if (Sprite) {
        Sprite->AlwaysLoadOnClient = true;
        Sprite->AlwaysLoadOnServer = true;
        Sprite->bOwnerNoSee = false;
        Sprite->bAffectDynamicIndirectLighting = true;
        Sprite->PrimaryComponentTick.TickGroup = TG_PrePhysics;
        Sprite->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
        static FName CollisionProfileName(TEXT("OverlapAllDynamic"));
        Sprite->SetCollisionProfileName(CollisionProfileName);
		Sprite->SetGenerateOverlapEvents(false);
		
        Sprite->SetFlipbook(IdleAnimation);

        // Enable replication on the Sprite component so animations show up when networked
        Sprite->SetIsReplicated(true);

        // Grab the socket
        UPaperFlipbook *flipBook = Sprite->GetFlipbook();
        UPaperSprite *sprite = flipBook->GetSpriteAtFrame(0); // this flipbook is only one frame long
        FPaperSpriteSocket *hitboxSocket = sprite->FindSocket(TEXT("HitboxSocket"));

        // World collision
        WorldCollisionBoxComponent = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("WorldCollisionBoxComponent"));
        WorldCollisionBoxComponent->SetRelativeTransform(hitboxSocket->LocalTransform);
        WorldCollisionBoxComponent->SetCollisionProfileName(CollisionProfileName);
		WorldCollisionBoxComponent->SetGenerateOverlapEvents(true);
        FVector boxExtent = FVector(7.8f, 7.8f, 7.8f); // 0.2f is a skin around the sprite
        WorldCollisionBoxComponent->InitBoxExtent(boxExtent);
        WorldCollisionBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

        // Hitbox
        HitBoxComponent = CreateOptionalDefaultSubobject<UBoxComponent>(TEXT("HitBoxComponent"));
        HitBoxComponent->SetRelativeTransform(hitboxSocket->LocalTransform);
        HitBoxComponent->SetCollisionProfileName(CollisionProfileName);
		HitBoxComponent->SetGenerateOverlapEvents(true);
        FVector hitboxExtent = FVector(3.8f, 3.8f, 3.8f); // 0.2f is a skin around the sprite
        HitBoxComponent->InitBoxExtent(hitboxExtent);
        HitBoxComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

        ConstructorHelpers::FObjectFinder<UParticleSystem> ArbitraryParticleName(TEXT("ParticleSystem'/Game/Particles/PuffEmitter.PuffEmitter'"));
        if (ArbitraryParticleName.Succeeded()) {
            PuffEmitter = ArbitraryParticleName.Object;
        }

    }

    // Set this pawn to be controlled by the lowest-numbered player
    //AutoPossessPlayer = EAutoReceiveInput::Player0; // commented out as this was causing issues in multiplayer

    if (UseOwnCamera) {
        // Create an orthographic camera (no perspective) and attach it to the boom
        SideViewCameraComponent = CreateDefaultSubobject<UPixelPerfectCameraComponent>(TEXT("SideViewCamera"));
        //SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
        //SideViewCameraComponent->SetAspectRatio(1.333334f);
        //SideViewCameraComponent->OrthoWidth = 320.0f;
        SideViewCameraComponent->bConstrainAspectRatio = true;
        SideViewCameraComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
        SideViewCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));
        SideViewCameraComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


        // Prevent all automatic rotation behavior on the camera, character, and camera component
        //SideViewCameraComponent->bUsePawnControlRotation = false;

        /*
        // Read the viewport size
        FVector2D viewportSize = FVector2D(1, 1);
        if (GEngine && GEngine->GameViewport) {
            GEngine->GameViewport->GetViewportSize(viewportSize);
            UE_LOG(LogTemp, Warning, TEXT("VX: %f"), viewportSize.X);
            UE_LOG(LogTemp, Warning, TEXT("VY: %f"), viewportSize.Y);
            SideViewCameraComponent->SetAspectRatio(viewportSize.X/viewportSize.Y);
            SideViewCameraComponent->OrthoWidth = viewportSize.X;
        }

        // Read the resolution
        FVector2D resolution = FVector2D( 1, 1 );
        resolution.X = GSystemResolution.ResX;
        resolution.Y = GSystemResolution.ResY;
        UE_LOG(LogTemp, Warning, TEXT("RX: %f"), resolution.X);
        UE_LOG(LogTemp, Warning, TEXT("RY: %f"), resolution.Y);
        SideViewCameraComponent->SetAspectRatio(resolution.X/resolution.Y);
        SideViewCameraComponent->OrthoWidth = resolution.X;
        */

        SideViewCameraComponent->DesignWidth = 40 * 16;
        SideViewCameraComponent->DesignHeight = 30 * 16;
        SideViewCameraComponent->BestFit();
    }

    Tags.Add(TEXT("Hero"));
    Tags.Add(TEXT("Solid"));

    Velocity = FVector(0, 0, 0);
    Scale = FVector(1, 1, 1);

    bReplicates = true;
    BulletFireSpeed = 10;

}

// Called when the game starts or when spawned
void AHeroPawn::BeginPlay() {
    Super::BeginPlay();
}

// Called every frame
void AHeroPawn::Tick(float DeltaTime) {
    Super::Tick(DeltaTime);

    // Init the controls
    float side = 0;
    float vert = 0;
    float jump = 0;
    float fire = 0;
    
    AFAPPlayerController *playerController = Cast<AFAPPlayerController>(GetController());
    int32 playerId = 0;
    if (playerController != nullptr) {
        playerId = playerController->GetLocalPlayer()->GetControllerId();
        if (playerId == 0) {
            side = InputComponent->GetAxisValue("MoveSide_P1");
            vert = InputComponent->GetAxisValue("MoveVertical_P1");
            jump = InputComponent->GetAxisValue("Jump_P1");
            fire = InputComponent->GetAxisValue("Fire_P1");
        } else {
            //UE_LOG(LogTemp, Warning, TEXT("Reading second player input"));
            side = InputComponent->GetAxisValue("MoveSide_P2");
            //UE_LOG(LogTemp, Warning, TEXT("Side: %f"), side);
            vert = InputComponent->GetAxisValue("MoveVertical_P2");
            jump = InputComponent->GetAxisValue("Jump_P2");
            fire = InputComponent->GetAxisValue("Fire_P2");
        }
    }

    InputWasLeft = InputLeft;
    InputWasRight = InputRight;
    InputWasDown = InputDown;
    InputWasJump = InputJump;
    InputWasFire = InputFire;
    InputLeft = /*LeftPressed;*/ side < 0 ? true : false;
    InputRight = /*RightPressed;*/ side > 0 ? true : false;
    InputDown = vert < 0 ? true : false;
    InputJump = jump > 0 ? true : false;
    InputFire = fire > 0 ? true : false;

    bool kLeft = InputLeft;
    bool kRight = InputRight;
    if (kRight) kLeft = false;
    bool kDown = InputDown;
    bool kJump = InputJump && !InputWasJump; // check that jump has just been pressed
    bool kJumpRelease = !InputJump && InputWasJump; // check that jump has just been released
    bool kFire = InputFire && !InputWasFire;

    // Temporary vars
    float tempAccel, tempFric;

    if (Dead) {
        kLeft = false;
        kRight = false;
        kDown = false;
        kJump = false;
        kFire = false;
    }

    // Reset the animation states;
    bIsFalling = false;
    bHasLanded = false;
    bIsJumping = false;
    bHasGrabbedWall = false;
    bIsShooting = false;
    bIsRunning = false;
    
    // Landed
    if (BottomCollided && !OnGroundPrev) {
        // Squash + stretch
        Scale.X = 1.5f;
        Scale.Z = 0.5f;
        if (PuffEmitter != nullptr) {
            UGameplayStatics::SpawnEmitterAttached(PuffEmitter, Sprite);
        }

        bHasLanded = true;

        /*
        var pos = transform.position;
        pos.y -= coll.bounds.size.y/2;
        EntityFactory.CreatePuff(pos);
         */
    }

    // Apply the correct form of acceleration and friction
    if (BottomCollided) {
        tempAccel = groundAccel;
        tempFric  = groundFric;
    } else {
        tempAccel = airAccel;
        tempFric  = airFric;
    }

    // Wall cling to avoid accidental push-off
    if ((!RightCollided && !LeftCollided) || BottomCollided) {
        canStick = true;
        sticking = false;
    } else if (((kRight && LeftCollided) || (kLeft && RightCollided)) && canStick && !BottomCollided) {
        sticking = true;
        canStick = false;
        bHasGrabbedWall = true;
    }

    if ((kRight || kLeft) && sticking) {
        canStick = true;
        sticking = false;
    }

    // Handle gravity
    if (!BottomCollided) {
        if ((LeftCollided || RightCollided) && Velocity.Z <= 0) {
            // Wall slide
            if(OnLeftPrev) Velocity.Z = 0;
            if(OnRightPrev) Velocity.Z = 0;

            Velocity.Z = Approach(Velocity.Z, -vyMax, -gravSlide);
            bIsFalling = true;
            bHasGrabbedWall = true;
        } else {
            // Fall normally
            Velocity.Z = Approach(Velocity.Z, -vyMax, -gravNorm);
            bIsFalling = true;
        }
    }

    // Left
    if (kLeft && !kRight && !sticking)
    {
        Facing = -1;
        Velocity.X = Approach(Velocity.X, -vxMax, tempAccel);
        bIsRunning = true;
        // Right
    } else if (kRight && !kLeft && !sticking) {
        Facing = 1;
        Velocity.X = Approach(Velocity.X, vxMax, tempAccel);
        bIsRunning = true;
    }

    // Friction
    if (!kRight && !kLeft /*&& physicsComponent.vx != 0*/) {
        Velocity.X = Approach(Velocity.X, 0, tempFric);
    }

    // Wall jump
    float jumpHeightStickY = jumpHeight * 1.1f;
    if (kJump && LeftCollided && !BottomCollided) {
        Scale.X = 0.5f;
        Scale.Z = 1.5f;
        // Wall jump is different when pushing off/towards the wall
        if (kLeft) {
            Velocity.X = jumpHeight * 0.25f;
            Velocity.Z = jumpHeightStickY;
        } else {
            Velocity.X = 0;//vxMax;
            Velocity.Z = jumpHeightStickY;
        }
        bIsJumping = true;
    } else if (kJump && RightCollided && !BottomCollided) {
        Scale.X = 0.5f;
        Scale.Z = 1.5f;
        // Wall jump is different when pushing off/towards the wall
        if (kRight) {
            Velocity.X = -jumpHeight * 0.25f;
            Velocity.Z = jumpHeightStickY;
        } else {
            Velocity.X = 0;//-vxMax;
            Velocity.Z = jumpHeightStickY;
        }
        bIsJumping = true;
    }
    // Fire
    if(kFire){
        Velocity.Z = 0;
        Scale.X = 0.5f;
        Scale.Z = 1.5f;
        // Shoot the ball if we're carrying it, otherwise fire the gun
        ABall *ball = FindBall();
        if (ball != nullptr && ball->Carrier == this) {
            ball->Shoot(Facing * 50);
        } else
        {
            // Shoot a bullet if we are not carrying the ball
            FireBullet();
        }
        bIsShooting = true;
    }

    // Ladders
    if (LadderCollided) {
        Velocity.Z = 0;
        if (InputJump) {
            Velocity.Z = jumpHeight * 0.2f;
        } else if (kDown) {
            Velocity.Z = -jumpHeight * 0.2f;
        }
    } else if (OnLadderPrev && !LadderCollided) {
        BottomCollided = true;
        Velocity.Z = 0;
        if (InputJump) kJump = true;
    }

    // Jump
    if (kJump) {
        if (LadderCollided) {
        } else if (BottomCollided) {
            doubleJumped = false;
            Scale.X = 0.5f;
            Scale.Z = 1.5f;
            Velocity.Z = jumpHeight;
            bIsJumping = true;
        } else if (!doubleJumped && !LeftCollided && !RightCollided) {
            Scale.X = 0.5f;
            Scale.Z = 1.5f;
            Velocity.Z = jumpHeight;
            doubleJumped = true;
            bIsJumping = true;
        }
        // Variable jumping
    } /*else if (kDown && !physicsComponent.bottomCollided) {
				physicsComponent.vy = -jumpHeight*2;
			}*/
    else if (kJumpRelease) {
        if (Velocity.Z > 0)
            Velocity.Z *= 0.25f;
    }

    // Swap the direction the sprite is facing if wall-grabbing
    // if (!BottomCollided) {
    //     if (LeftCollided)
    //         Facing = 1;
    //     if (RightCollided)
    //         Facing = -1;
    // }

    Scale.X = Approach(Scale.X, 1.0f, 0.05f);
    Scale.Z = Approach(Scale.Z, 1.0f, 0.05f);

    MoveH(Velocity.X * TimeMult /*DeltaTime * 50*/);
    MoveV(Velocity.Z * TimeMult /*DeltaTime * 50*/);
    //MoveH(Velocity.X * DeltaTime * 50);
    //MoveV(Velocity.Z * DeltaTime * 50);

    Sprite->SetRelativeScale3D(Scale);
    FVector spriteScale = Sprite->GetComponentScale();
    spriteScale.X *= Facing;
    Sprite->SetRelativeScale3D(spriteScale);

    OnGroundPrev = BottomCollided;
    OnTopPrev = TopCollided;
    OnLeftPrev = LeftCollided;
    OnRightPrev = RightCollided;
    OnLadderPrev = LadderCollided;

    BottomCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X, GetActorLocation().Z-1) != nullptr;
    LeftCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X-1, GetActorLocation().Z) != nullptr;
    TopCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X, GetActorLocation().Z+1) != nullptr;
    RightCollided = CollideFirst(TEXT("Solid"), GetActorLocation().X+1, GetActorLocation().Z) != nullptr;

    UpdateAnimation();

    /*
     * Old movement code
    if (!CurrentVelocity.IsZero()) {
        FVector NewLocation = GetActorLocation() + (CurrentVelocity * DeltaTime);
        SetActorLocation(NewLocation);
    }

    TArray<AActor *> actors;
    Sprite->GetOverlappingActors(actors, NULL);
    // then merge it into our final list
    for (auto CompIt = actors.CreateIterator(); CompIt; ++CompIt) {
        UE_LOG(LogTemp, Warning, TEXT("Overlapping actor."));
        AActor *OverlappingActor = *CompIt;
        if (OverlappingActor != this) {
            UE_LOG(LogTemp, Warning, TEXT("Actor: %s"), *AActor::GetDebugName(OverlappingActor));
            //CurrentVelocity.Z = 100;
        }
    }
    */
}

// Called to bind functionality to input
void AHeroPawn::SetupPlayerInputComponent(class UInputComponent * PlayerInputComponent) {
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    AController *c = GetController();
    if (c == nullptr) {
        UE_LOG(LogTemp, Warning, TEXT("MISSING CONTROLLER"));
    } else {
        UE_LOG(LogTemp, Warning, TEXT("CONTROLLER: %s"), *AActor::GetDebugName(c));
    }

    // Find out which player we are
    AFAPPlayerController *playerController = Cast<AFAPPlayerController>(GetController());
    PlayerId = 0;
    if (playerController != nullptr) {
        PlayerId = playerController->GetLocalPlayer()->GetControllerId();
        UE_LOG(LogTemp, Warning, TEXT("Player Controller Id: %d"), PlayerId);
    } else {
        UE_LOG(LogTemp, Warning, TEXT("The pawn has no controller"));
    }

    UE_LOG(LogTemp, Warning, TEXT("SetupPlayerInputComponent says you are player %d"), PlayerId);

    if (PlayerId == 0) {
        PlayerInputComponent->BindAxis("MoveSide_P1", this, &AHeroPawn::MoveSide);
        PlayerInputComponent->BindAxis("MoveVertical_P1", this, &AHeroPawn::MoveVertical);
        PlayerInputComponent->BindAxis("Jump_P1", this, &AHeroPawn::Jump);
        PlayerInputComponent->BindAxis("Fire_P1", this, &AHeroPawn::Fire);
    } else {
        PlayerInputComponent->BindAxis("MoveSide_P2", this, &AHeroPawn::MoveSide);
        PlayerInputComponent->BindAxis("MoveVertical_P2", this, &AHeroPawn::MoveVertical);
        PlayerInputComponent->BindAxis("Jump_P2", this, &AHeroPawn::Jump);
        PlayerInputComponent->BindAxis("Fire_P2", this, &AHeroPawn::Fire);
    }

    // Reset the correct idle animation based on the player id
    if (PlayerId == 1) {
        IdleAnimation = LoadObject<UPaperFlipbook>(NULL, TEXT("/Game/Animations/HeroBlueIdle.HeroBlueIdle"), NULL, LOAD_None, NULL);
        Sprite->SetFlipbook(IdleAnimation);
    }


    // Local Player 1
    //InputComponent->BindAxis("MoveSide", this, &AHeroPawn::MoveSide);
    //InputComponent->BindAxis("MoveVertical", this, &AHeroPawn::MoveVertical);
    //InputComponent->BindAxis("Jump", this, &AHeroPawn::Jump);

    // Local Player 2
    //InputComponent->BindAxis("MoveSide2", this, &AHeroPawn::MoveSide);
    //InputComponent->BindAxis("MoveVertical2", this, &AHeroPawn::MoveVertical);
    //InputComponent->BindAxis("Jump2", this, &AHeroPawn::Jump);
    
    /*
    // Testing spawns
    InputComponent->BindAction("SpawnBunny", EInputEvent::IE_Released, this, &AHeroPawn::SpawnBunnies);

    // Those are for testing only
    InputComponent->BindAction("MoveRight", EInputEvent::IE_Pressed, this, &AHeroPawn::MoveRight);
    InputComponent->BindAction("MoveLeft", EInputEvent::IE_Pressed, this, &AHeroPawn::MoveLeft);
    InputComponent->BindAction("MoveRight", EInputEvent::IE_Released, this, &AHeroPawn::StopRight);
    InputComponent->BindAction("MoveLeft", EInputEvent::IE_Released, this, &AHeroPawn::StopLeft);
    */
}

void AHeroPawn::MoveSide(float Value) {
    //MoveH(Value);


/*
    // Move at 100 units per second forward or backward
    CurrentVelocity.X = FMath::Clamp(Value, -1.0f, 1.0f) * 100.0f;
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, FString::Printf(TEXT("Value: %f"), Value));
    GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, FString::Printf(TEXT("VX: %f"), CurrentVelocity.X));

    if (Value > 0.0f) {
        UE_LOG(LogTemp, Warning, TEXT("MOVE RIGHT"));
    } else if (Value < 0.0f) {
        UE_LOG(LogTemp, Warning, TEXT("MOVE LEFT"));
    }
*/
    // Update animation to match the motion
    //UpdateAnimation();

    // Set the rotation so that the character faces his direction of travel.
    /*
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
     */
}

void AHeroPawn::MoveVertical(float Value) {
    //MoveV(Value);
}

void AHeroPawn::Jump(float Value) {
}

void AHeroPawn::Fire(float Value) {
}

void AHeroPawn::SpawnBunnies() {
    for (TActorIterator<ABunnyManager> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
        ActorItr->AddBunnies(1000);
    }
}

void AHeroPawn::MoveLeft() {
    LeftPressed = true;
}

void AHeroPawn::MoveRight() {
    RightPressed = true;
}

void AHeroPawn::StopLeft() {
    LeftPressed = false;
}

void AHeroPawn::StopRight() {
    RightPressed = false;
}

void AHeroPawn::NotifyActorBeginOverlap(AActor *OtherActor) {
    AActor::NotifyActorBeginOverlap(OtherActor);
    //UE_LOG(LogTemp, Warning, TEXT("BEGIN OVERLAP"));
}

ABall *AHeroPawn::FindBall() const
{
    ABall *Ball = nullptr;
    for (TActorIterator<ABall> ActorItr(GetWorld()); ActorItr; ++ActorItr) {
        Ball =  Cast<ABall>(*ActorItr);
    }
    return Ball;
}

void AHeroPawn::FireBullet()
{
    if (BulletClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Trying to fire a bullet with undefined class. Assign BulletClass to this actor."));
        return;
    }

    UPaperFlipbook *FlipBook = Sprite->GetFlipbook();
    UPaperSprite *FirstSprite = FlipBook->GetSpriteAtFrame(0);
    FPaperSpriteSocket *WeaponSocket = FirstSprite->FindSocket(TEXT("WeaponSocket"));
    FVector SpawnLocation;
    
    if (WeaponSocket == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("No WeaponSocket found in this Sprite. Using Actor Location."));
        SpawnLocation = GetActorLocation();
    } else
    {
        FVector FacingVector = WeaponSocket->LocalTransform.GetLocation();
        FacingVector.X *= Facing;
        SpawnLocation = GetActorLocation() + FacingVector;
    }
    
    ABullet *Bullet = GetWorld()->SpawnActor<ABullet>(BulletClass, SpawnLocation, FRotator(), FActorSpawnParameters());
    Bullet->Shoot(BulletFireSpeed);
}

void AHeroPawn::ApplyDamage(float Amount)
{
    if (CombatTextClass == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("Missing CombatTextClass. Please assign it in the BP."));
        return;
    }
    APopText *Pop = GetWorld()->SpawnActor<APopText>(CombatTextClass, GetActorLocation(),FRotator(), FActorSpawnParameters());
    Pop->SetContent(FString::FromInt(Amount));
}

void AHeroPawn::UpdateAnimation()
{
    if (bIsShooting)
    {
        Sprite->SetFlipbook(ShootAnimation);
        Sprite->SetLooping(false);
        Sprite->Play();
    }
    else if (bIsJumping)
    {
        Sprite->SetFlipbook(JumpAnimation);
        Sprite->SetLooping(false);
        Sprite->Play();
    }
    else if (bHasGrabbedWall)
    {
        Sprite->SetFlipbook(WallGrabAnimation);
        Sprite->SetLooping(false);
        Sprite->Play();
    }
    else if (bIsFalling)
    {
        Sprite->SetFlipbook(FallAnimation);
        Sprite->SetLooping(false);
        Sprite->Play();
    }
    else if (bHasLanded)
    {
        Sprite->SetFlipbook(IdleAnimation);
        Sprite->SetLooping(false);
        Sprite->Play();
    }
    else if (bIsRunning)
    {
        Sprite->SetFlipbook(RunAnimation);
        Sprite->SetLooping(true);
        Sprite->Play();
    } else
    {
        Sprite->SetFlipbook(IdleAnimation);
        Sprite->SetLooping(false);
        Sprite->Play();
    }
}
