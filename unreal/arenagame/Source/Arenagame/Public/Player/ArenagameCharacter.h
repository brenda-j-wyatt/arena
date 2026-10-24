#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArenagameCharacter.generated.h"

class UCameraComponent;
class USpotLightComponent;
class USurvivalStatusComponent;
class UInputAction;
class UInputMappingContext;
class UCameraShakeBase;
class UFearSanityManagerComponent;
struct FInputActionValue;

UCLASS(Blueprintable)
class ARENAGAME_API AArenagameCharacter : public ACharacter
{
    GENERATED_BODY()

public:
    AArenagameCharacter();

    virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<UCameraComponent> FirstPersonCamera;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<USpotLightComponent> Flashlight;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<USurvivalStatusComponent> SurvivalStatus;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<UFearSanityManagerComponent> FearSanityManager;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float WalkSpeed = 300.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float SprintSpeed = 520.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float StaminaDrainPerSecond = 24.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement")
    float StaminaRegenPerSecond = 18.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flashlight")
    float FlashlightBatteryDrainPerSecond = 3.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    float InteractionDistance = 250.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Camera")
    TSubclassOf<UCameraShakeBase> FearCameraShake;

    UFUNCTION(BlueprintCallable, Category="Player|Flashlight")
    void SetFlashlightEnabled(bool bEnabled);
    UFUNCTION(BlueprintPure, Category="Player|Flashlight")
    bool IsFlashlightEnabled() const { return bFlashlightEnabled; }
    UFUNCTION(BlueprintCallable, Category="Player|Interaction")
    void TryInteract();
    UFUNCTION(BlueprintCallable, Category="Player|Noise")
    void EmitNoise(float Loudness = 1.0f);

protected:
    virtual void BeginPlay() override;
    void MoveForward(const FInputActionValue& Value);
    void MoveRight(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void StartJump();
    void StopJump();
    void StartSprint();
    void StopSprint();
    void ToggleCrouch();
    void ToggleFlashlight();
    void UpdateFearCameraShake();
    void CreateRuntimeInput(UInputComponent* PlayerInputComponent);

private:
    UPROPERTY(Transient)
    TObjectPtr<UInputMappingContext> RuntimeMappingContext;
    UPROPERTY(Transient)
    TArray<TObjectPtr<UInputAction>> RuntimeActions;
    bool bWantsToSprint = false;
    bool bFlashlightEnabled = false;
    bool bFearShakeActive = false;
};
