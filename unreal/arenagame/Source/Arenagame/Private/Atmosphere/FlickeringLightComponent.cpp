#include "Atmosphere/FlickeringLightComponent.h"
#include "Components/LightComponent.h"
#include "TimerManager.h"
#include "Engine/World.h"

UFlickeringLightComponent::UFlickeringLightComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UFlickeringLightComponent::BeginPlay()
{
    Super::BeginPlay();
    Light = GetOwner() ? GetOwner()->FindComponentByClass<ULightComponent>() : nullptr;
    if (Light) OriginalIntensity = Light->Intensity;
    SetFlickering(bStartEnabled);
}

void UFlickeringLightComponent::SetFlickering(bool bEnabled)
{
    bFlickering = bEnabled;
    if (!GetWorld()) return;
    GetWorld()->GetTimerManager().ClearTimer(FlickerTimer);
    if (bFlickering) GetWorld()->GetTimerManager().SetTimer(FlickerTimer, this, &UFlickeringLightComponent::Flicker, FMath::Max(0.01f, FlickerInterval), true);
    else if (Light) Light->SetIntensity(OriginalIntensity);
}

void UFlickeringLightComponent::Flicker()
{
    if (Light && FMath::FRand() < FlickerChance)
    {
        const float Multiplier = FMath::FRand() < 0.5f ? MinimumIntensityMultiplier : 1.0f;
        Light->SetIntensity(OriginalIntensity * Multiplier);
    }
}
