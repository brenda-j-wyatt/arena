#include "Player/ArenagameCharacter.h"
#include "Player/SurvivalStatusComponent.h"
#include "Horror/FearSanityManagerComponent.h"
#include "Horror/StalkerEntity.h"
#include "EngineUtils.h"
#include "Interaction/InteractionInterface.h"
#include "Components/CapsuleComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/SpotLightComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputActionValue.h"
#include "InputMappingContext.h"
#include "InputModifiers.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Engine/LocalPlayer.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "InputCoreTypes.h"

AArenagameCharacter::AArenagameCharacter()
{
    PrimaryActorTick.bCanEverTick = true;
    GetCapsuleComponent()->InitCapsuleSize(34.0f, 88.0f);
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    GetCharacterMovement()->NavAgentProps.bCanCrouch = true;

    FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
    FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
    FirstPersonCamera->SetRelativeLocation(FVector(0.0f, 0.0f, 64.0f));
    FirstPersonCamera->bUsePawnControlRotation = true;

    Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
    Flashlight->SetupAttachment(FirstPersonCamera);
    Flashlight->SetRelativeLocation(FVector(16.0f, 0.0f, 0.0f));
    Flashlight->SetIntensity(9000.0f);
    Flashlight->SetAttenuationRadius(1800.0f);
    Flashlight->SetVisibility(false);

    SurvivalStatus = CreateDefaultSubobject<USurvivalStatusComponent>(TEXT("SurvivalStatus"));
    FearSanityManager = CreateDefaultSubobject<UFearSanityManagerComponent>(TEXT("FearSanityManager"));
}

void AArenagameCharacter::BeginPlay()
{
    Super::BeginPlay();
    GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
    SetFlashlightEnabled(false);
}

void AArenagameCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    if (bWantsToSprint && GetVelocity().SizeSquared2D() > 25.0f && SurvivalStatus->Stamina > 0.0f)
    {
        SurvivalStatus->SetStamina(SurvivalStatus->Stamina - StaminaDrainPerSecond * DeltaSeconds);
        if (SurvivalStatus->Stamina <= 0.0f) bWantsToSprint = false;
    }
    else
    {
        SurvivalStatus->SetStamina(SurvivalStatus->Stamina + StaminaRegenPerSecond * DeltaSeconds);
    }

    if (bFlashlightEnabled)
    {
        SurvivalStatus->ConsumeBattery(FlashlightBatteryDrainPerSecond * DeltaSeconds);
        if (SurvivalStatus->Battery <= 0.0f) SetFlashlightEnabled(false);
    }

    const float FearMultiplier = SurvivalStatus->GetFearSpeedMultiplier();
    const bool bCanSprintNow = bWantsToSprint && SurvivalStatus->Stamina > 0.0f;
    GetCharacterMovement()->MaxWalkSpeed = (bCanSprintNow ? SprintSpeed : WalkSpeed) * FearMultiplier;
    UpdateFearCameraShake();
}

void AArenagameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);
    CreateRuntimeInput(PlayerInputComponent);
}

void AArenagameCharacter::CreateRuntimeInput(UInputComponent* PlayerInputComponent)
{
    UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
    if (!EnhancedInput) return;

    RuntimeMappingContext = NewObject<UInputMappingContext>(this, TEXT("RuntimePlayerMapping"));
    auto MakeAction = [this](const TCHAR* Name, EInputActionValueType Type)
    {
        UInputAction* Action = NewObject<UInputAction>(this, FName(Name));
        Action->ValueType = Type;
        RuntimeActions.Add(Action);
        return Action;
    };

    UInputAction* Forward = MakeAction(TEXT("MoveForward"), EInputActionValueType::Axis1D);
    UInputAction* Right = MakeAction(TEXT("MoveRight"), EInputActionValueType::Axis1D);
    UInputAction* LookAction = MakeAction(TEXT("Look"), EInputActionValueType::Axis2D);
    UInputAction* JumpAction = MakeAction(TEXT("Jump"), EInputActionValueType::Boolean);
    UInputAction* SprintAction = MakeAction(TEXT("Sprint"), EInputActionValueType::Boolean);
    UInputAction* CrouchAction = MakeAction(TEXT("Crouch"), EInputActionValueType::Boolean);
    UInputAction* FlashlightAction = MakeAction(TEXT("Flashlight"), EInputActionValueType::Boolean);
    UInputAction* InteractAction = MakeAction(TEXT("Interact"), EInputActionValueType::Boolean);

    RuntimeMappingContext->MapKey(Forward, EKeys::W);
    RuntimeMappingContext->MapKey(Forward, EKeys::S).Modifiers.Add(NewObject<UInputModifierNegate>(RuntimeMappingContext));
    RuntimeMappingContext->MapKey(Right, EKeys::D);
    RuntimeMappingContext->MapKey(Right, EKeys::A).Modifiers.Add(NewObject<UInputModifierNegate>(RuntimeMappingContext));
    RuntimeMappingContext->MapKey(LookAction, EKeys::Mouse2D);
    RuntimeMappingContext->MapKey(JumpAction, EKeys::SpaceBar);
    RuntimeMappingContext->MapKey(SprintAction, EKeys::LeftShift);
    RuntimeMappingContext->MapKey(CrouchAction, EKeys::LeftControl);
    RuntimeMappingContext->MapKey(FlashlightAction, EKeys::F);
    RuntimeMappingContext->MapKey(InteractAction, EKeys::E);

    EnhancedInput->BindAction(Forward, ETriggerEvent::Triggered, this, &AArenagameCharacter::MoveForward);
    EnhancedInput->BindAction(Right, ETriggerEvent::Triggered, this, &AArenagameCharacter::MoveRight);
    EnhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &AArenagameCharacter::Look);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &AArenagameCharacter::StartJump);
    EnhancedInput->BindAction(JumpAction, ETriggerEvent::Completed, this, &AArenagameCharacter::StopJump);
    EnhancedInput->BindAction(SprintAction, ETriggerEvent::Started, this, &AArenagameCharacter::StartSprint);
    EnhancedInput->BindAction(SprintAction, ETriggerEvent::Completed, this, &AArenagameCharacter::StopSprint);
    EnhancedInput->BindAction(CrouchAction, ETriggerEvent::Started, this, &AArenagameCharacter::ToggleCrouch);
    EnhancedInput->BindAction(FlashlightAction, ETriggerEvent::Started, this, &AArenagameCharacter::ToggleFlashlight);
    EnhancedInput->BindAction(InteractAction, ETriggerEvent::Started, this, &AArenagameCharacter::TryInteract);

    if (APlayerController* PC = Cast<APlayerController>(GetController()))
    {
        if (ULocalPlayer* LocalPlayer = PC->GetLocalPlayer())
        {
            if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
            {
                Subsystem->AddMappingContext(RuntimeMappingContext, 0);
            }
        }
    }
}

void AArenagameCharacter::MoveForward(const FInputActionValue& Value)
{
    AddMovementInput(GetActorForwardVector(), Value.Get<float>());
}

void AArenagameCharacter::MoveRight(const FInputActionValue& Value)
{
    AddMovementInput(GetActorRightVector(), Value.Get<float>());
}

void AArenagameCharacter::Look(const FInputActionValue& Value)
{
    const FVector2D Axis = Value.Get<FVector2D>();
    AddControllerYawInput(Axis.X);
    AddControllerPitchInput(Axis.Y);
}

void AArenagameCharacter::StartJump() { Jump(); }
void AArenagameCharacter::StopJump() { StopJumping(); }
void AArenagameCharacter::StartSprint() { bWantsToSprint = SurvivalStatus->Stamina > 0.0f; }
void AArenagameCharacter::StopSprint() { bWantsToSprint = false; }

void AArenagameCharacter::ToggleCrouch()
{
    if (bIsCrouched) UnCrouch(); else Crouch();
}

void AArenagameCharacter::ToggleFlashlight()
{
    SetFlashlightEnabled(!bFlashlightEnabled);
}

void AArenagameCharacter::SetFlashlightEnabled(bool bEnabled)
{
    bFlashlightEnabled = bEnabled && SurvivalStatus && SurvivalStatus->Battery > 0.0f;
    if (Flashlight) Flashlight->SetVisibility(bFlashlightEnabled);
}

void AArenagameCharacter::TryInteract()
{
    if (!FirstPersonCamera) return;
    const FVector Start = FirstPersonCamera->GetComponentLocation();
    const FVector End = Start + FirstPersonCamera->GetForwardVector() * InteractionDistance;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(PlayerInteraction), false, this);
    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        if (AActor* Target = Hit.GetActor(); Target && Target->GetClass()->ImplementsInterface(UInteractionInterface::StaticClass()))
        {
            IInteractionInterface::Execute_Interact(Target, this);
        }
    }
}

void AArenagameCharacter::EmitNoise(float Loudness)
{
    const float HearingRadius = 1200.0f * FMath::Max(0.1f, Loudness);
    for (TActorIterator<AStalkerEntity> It(GetWorld()); It; ++It)
    {
        if (FVector::DistSquared(GetActorLocation(), It->GetActorLocation()) <= FMath::Square(HearingRadius))
        {
            It->ReportNoise(GetActorLocation(), Loudness);
        }
    }
}

void AArenagameCharacter::UpdateFearCameraShake()
{
    if (!FearCameraShake || !Controller) return;
    const float Intensity = SurvivalStatus->GetCameraShakeIntensity();
    if (APlayerController* PC = Cast<APlayerController>(Controller))
    {
        if (Intensity > 0.05f && !bFearShakeActive)
        {
            PC->ClientStartCameraShake(FearCameraShake, Intensity);
            bFearShakeActive = true;
        }
        else if (Intensity <= 0.01f)
        {
            PC->ClientStopCameraShake(FearCameraShake, false);
            bFearShakeActive = false;
        }
    }
}
