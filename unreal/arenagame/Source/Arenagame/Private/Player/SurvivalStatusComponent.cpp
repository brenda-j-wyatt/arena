#include "Player/SurvivalStatusComponent.h"

USurvivalStatusComponent::USurvivalStatusComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void USurvivalStatusComponent::SetStamina(float NewValue)
{
    Stamina = FMath::Clamp(NewValue, 0.0f, MaxStamina);
}

void USurvivalStatusComponent::ConsumeBattery(float Amount)
{
    Battery = FMath::Clamp(Battery - FMath::Max(0.0f, Amount), 0.0f, MaxBattery);
}

void USurvivalStatusComponent::SetFear(float NewValue)
{
    const float Clamped = FMath::Clamp(NewValue, 0.0f, 100.0f);
    if (!FMath::IsNearlyEqual(Fear, Clamped))
    {
        Fear = Clamped;
        OnFearChanged.Broadcast(Fear);
    }
}

void USurvivalStatusComponent::ApplyFearDelta(float Delta)
{
    SetFear(Fear + Delta);
}

void USurvivalStatusComponent::ApplySanityDelta(float Delta)
{
    Sanity = FMath::Clamp(Sanity + Delta, 0.0f, MaxSanity);
}

float USurvivalStatusComponent::GetFearSpeedMultiplier() const
{
    return FMath::Lerp(1.0f, MinimumFearSpeedMultiplier, Fear / 100.0f);
}

float USurvivalStatusComponent::GetCameraShakeIntensity() const
{
    return FMath::Clamp(Fear / 100.0f, 0.0f, 1.0f) * CameraShakeAtMaxFear;
}

float USurvivalStatusComponent::GetHeartbeatVolume() const
{
    return FMath::Clamp(Fear / 100.0f, 0.0f, 1.0f) * HeartbeatVolumeAtMaxFear;
}
