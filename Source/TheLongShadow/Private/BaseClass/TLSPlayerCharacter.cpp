// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseClass/TLSPlayerCharacter.h"
#include "Engine/LocalPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Interface/InteractionInterface.h"
#include "Logger.h"

void ATLSPlayerCharacter::TraceForInteraction()
{
    FCollisionQueryParams LTParams = FCollisionQueryParams(FName(TEXT("InteractionTrace")), true, this);
    LTParams.bReturnPhysicalMaterial = false;
    LTParams.bReturnFaceIndex = false;
    GetWorld()->DebugDrawTraceTag = DEBUG_SHOW_INTERACTION_TRACE ? TEXT("InteractionTrace") : GetWorld()->DebugDrawTraceTag = TEXT("NONE");
    FHitResult LTHit(ForceInit);
    FVector LTStart = FollowCamera->GetComponentLocation();
    float SearchLength = (FollowCamera->GetComponentLocation() - CameraBoom->GetComponentLocation()).Length();
    SearchLength += InteractionTraceLength;
    FVector LTEnd = (FollowCamera->GetForwardVector() * SearchLength) + LTStart;

    GetWorld()->LineTraceSingleByChannel(LTHit, LTStart, LTEnd, ECC_Visibility, LTParams);

    UpdateInteractionText_Implementation();
    if (!LTHit.bBlockingHit || !LTHit.GetActor()->Implements<UInteractionInterface>())
    {
        InteractionActor = nullptr;
        return;
    }
    InteractionActor = LTHit.GetActor();
}
void ATLSPlayerCharacter::Move(const FInputActionValue &Value)
{
    // input is a Vector2D
    FVector2D MovementVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // find out which way is forward
        const FRotator Rotation = Controller->GetControlRotation();
        const FRotator YawRotation(0, Rotation.Yaw, 0);

        // get forward vector
        const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

        // get right vector
        const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

        // add movement
        AddMovementInput(ForwardDirection, MovementVector.Y);
        AddMovementInput(RightDirection, MovementVector.X);
    }
}
void ATLSPlayerCharacter::Look(const FInputActionValue &Value)
{
    // input is a Vector2D
    FVector2D LookAxisVector = Value.Get<FVector2D>();

    if (Controller != nullptr)
    {
        // add yaw and pitch input to controller
        AddControllerYawInput(LookAxisVector.X);
        AddControllerPitchInput(LookAxisVector.Y);
    }
}
void ATLSPlayerCharacter::PlayerJump()
{
    if (ATLSCharacter::CanJump())
    {
        ATLSCharacter::HasJumped();
    }
}
void ATLSPlayerCharacter::SprintOn()
{
    SetSprinting(true);
}
void ATLSPlayerCharacter::SprintOff()
{
    SetSprinting(false);
}
void ATLSPlayerCharacter::SneakOn()
{
    SetSneaking(true);
}
void ATLSPlayerCharacter::SneakOff()
{
    SetSneaking(false);
}
void ATLSPlayerCharacter::OnInteract()
{
    if (InteractionActor == nullptr)
    {
        return;
    }
    IInteractionInterface *Inter = Cast<IInteractionInterface>(InteractionActor);
    if (Inter == nullptr)
    {
        Logger::GetInstance()->AddMessage("ALTSPlayerCharacter::OnInteract - Failed to cast to InteractionInterface", ERRORLEVEL::EL_ERROR);
        return;
    }
    // Inter->Interact_Implementation(this);
    Inter->Execute_Interact(InteractionActor, this);
}
void ATLSPlayerCharacter::NotifyControllerChanged()
{
    Super::NotifyControllerChanged();

    // Add Input Mapping Context
    if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
    {
        if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
        {
            Subsystem->AddMappingContext(DefaultMappingContext, 0);
        }
    }
}
void ATLSPlayerCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
    // Set up action bindings
    if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
    {

        // Jumping
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ATLSPlayerCharacter::PlayerJump);
        EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

        // Moving
        EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ATLSPlayerCharacter::Move);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ATLSPlayerCharacter::SprintOn);
        EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ATLSPlayerCharacter::SprintOff);
        EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Started, this, &ATLSPlayerCharacter::SneakOn);
        EnhancedInputComponent->BindAction(SneakAction, ETriggerEvent::Completed, this, &ATLSPlayerCharacter::SneakOff);
        EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Completed, this, &ATLSPlayerCharacter::OnInteract);

        // Looking
        EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ATLSPlayerCharacter::Look);
    }
}
void ATLSPlayerCharacter::BeginPlay()
{
    Super::BeginPlay();
    // ATLSCharacter::BeginPlay();
    SaveActorID.Invalidate();
}
ATLSPlayerCharacter::ATLSPlayerCharacter()
{
    // Set size for collision capsule
    GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

    // Don't rotate when the controller rotates. Let that just affect the camera.
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = false;
    bUseControllerRotationRoll = false;

    // Configure character movement
    GetCharacterMovement()->bOrientRotationToMovement = true;            // Character moves in the direction of input...
    GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

    // Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
    // instead of recompiling to adjust them
    GetCharacterMovement()->JumpZVelocity = 700.f;
    GetCharacterMovement()->AirControl = 0.35f;
    GetCharacterMovement()->MaxWalkSpeed = 500.f;
    GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
    GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
    GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

    // Create a camera boom (pulls in towards the player if there is a collision)
    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 400.0f;       // The camera follows at this distance behind the character
    CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

    // Create a follow camera
    FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
    FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
    FollowCamera->bUsePawnControlRotation = false;                              // Camera does not rotate relative to arm

    // Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character)
    // are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

    InteractionTrigger = CreateDefaultSubobject<USphereComponent>(TEXT("Interaction Trigger Volume"));
    InteractionTrigger->SetupAttachment(RootComponent);
    InteractionTrigger->SetRelativeScale3D(FVector(10));
    InteractionTrigger->OnComponentBeginOverlap.AddDynamic(this, &ATLSPlayerCharacter::OnInteractionTriggerOverlapBegin);
    InteractionTrigger->OnComponentEndOverlap.AddDynamic(this, &ATLSPlayerCharacter::OnInteractionTriggerOverlapEnd);

    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bTickEvenWhenPaused = false;
}
void ATLSPlayerCharacter::Tick(float DeltaTime)
{
    if (bEnableRayTrace)
    {
        TraceForInteraction();
    }
}
void ATLSPlayerCharacter::OnInteractionTriggerOverlapBegin(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
    if (!OtherActor->Implements<UInteractionInterface>())
    {
        return;
    }
    InteractableActors.Add(OtherActor);
    bEnableRayTrace = true;
}

void ATLSPlayerCharacter::OnInteractionTriggerOverlapEnd(UPrimitiveComponent *OverlappedComponent, AActor *OtherActor, UPrimitiveComponent *OtherComponent, int32 OtherBodyIndex)
{
    if (!OtherActor->Implements<UInteractionInterface>())
    {
        return;
    }
    InteractableActors.Remove(OtherActor);
    bEnableRayTrace = InteractableActors.Num() > 0;
}

void ATLSPlayerCharacter::UpdateInteractionText_Implementation()
{
    UpdateInteractionText();
}
