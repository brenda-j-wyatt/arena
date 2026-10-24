#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SurvivalStatusComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFearChanged, float, Fear);

UCLASS(ClassGroup=(Arenagame), meta=(BlueprintSpawnableComponent))
class ARENAGAME_API USurvivalStatusComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    USurvivalStatusComponent();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Survival", meta=(ClampMin="0.0"))
    float MaxHealth = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Survival")
    float Health = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Survival", meta=(ClampMin="1.0"))
    float MaxStamina = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Survival")
    float Stamina = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Survival", meta=(ClampMin="0.0"))
    float MaxBattery = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Survival")
    float Battery = 100.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Survival", meta=(ClampMin="0.0"))
    float MaxSanity = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Survival")
    float Sanity = 100.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Survival", meta=(ClampMin="0.0", ClampMax="100.0"))
    float Fear = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear", meta=(ClampMin="0.1", ClampMax="1.0"))
    float MinimumFearSpeedMultiplier = 0.72f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear")
    float CameraShakeAtMaxFear = 1.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear")
    float HeartbeatVolumeAtMaxFear = 1.0f;

    UPROPERTY(BlueprintAssignable, Category="Fear")
    FOnFearChanged OnFearChanged;

    UFUNCTION(BlueprintCallable, Category="Survival")
    void SetStamina(float NewValue);
    UFUNCTION(BlueprintCallable, Category="Survival")
    void ConsumeBattery(float Amount);
    UFUNCTION(BlueprintCallable, Category="Survival")
    void SetFear(float NewValue);
    UFUNCTION(BlueprintCallable, Category="Survival")
    void ApplyFearDelta(float Delta);
    UFUNCTION(BlueprintCallable, Category="Survival")
    void ApplySanityDelta(float Delta);
    UFUNCTION(BlueprintPure, Category="Fear")
    float GetFearSpeedMultiplier() const;
    UFUNCTION(BlueprintPure, Category="Fear")
    float GetCameraShakeIntensity() const;
    UFUNCTION(BlueprintPure, Category="Fear")
    float GetHeartbeatVolume() const;
};
