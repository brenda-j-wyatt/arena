#include "Atmosphere/StormLightingController.h"
#include "Engine/DirectionalLight.h"
#include "Components/DirectionalLightComponent.h"
#include "Engine/ExponentialHeightFog.h"
#include "Components/ExponentialHeightFogComponent.h"

AStormLightingController::AStormLightingController()
{
    PrimaryActorTick.bCanEverTick = true;
}

void AStormLightingController::SetStormIntensity(float NormalizedIntensity)
{
    IntensityScale = FMath::Clamp(NormalizedIntensity, 0.0f, 2.0f);
}

void AStormLightingController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);
    ElapsedTime += DeltaSeconds;
    const float Pulse = 1.0f + FMath::Sin(ElapsedTime * StormPulseSpeed) * StormPulseAmount;
    if (DirectionalLight && DirectionalLight->GetLightComponent())
    {
        DirectionalLight->GetLightComponent()->SetIntensity(StormLightIntensity * IntensityScale * Pulse);
    }
    if (ExponentialFog && ExponentialFog->GetComponent())
    {
        ExponentialFog->GetComponent()->SetFogDensity(FMath::Max(0.0f, StormFogDensity * IntensityScale));
    }
}
