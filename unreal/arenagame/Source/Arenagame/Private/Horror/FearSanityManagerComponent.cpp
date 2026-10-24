#include "Horror/FearSanityManagerComponent.h"
#include "Player/SurvivalStatusComponent.h"
#include "Player/ArenagameCharacter.h"
#include "Horror/StalkerEntity.h"
#include "Engine/World.h"
#include "EngineUtils.h"
#include "DrawDebugHelpers.h"

UFearSanityManagerComponent::UFearSanityManagerComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UFearSanityManagerComponent::BeginPlay()
{
    Super::BeginPlay();
    Status = GetOwner() ? GetOwner()->FindComponentByClass<USurvivalStatusComponent>() : nullptr;
}

void UFearSanityManagerComponent::SetDarknessExposure(float Exposure)
{
    DarknessExposure = FMath::Clamp(Exposure, 0.0f, 1.0f);
    bHasDarknessOverride = true;
}

void UFearSanityManagerComponent::ClearDarknessExposureOverride()
{
    bHasDarknessOverride = false;
}

void UFearSanityManagerComponent::ReportScriptedScare(float FearAmount, float SanityDamage)
{
    if (Status)
    {
        Status->ApplyFearDelta(FMath::Max(0.0f, FearAmount));
        Status->ApplySanityDelta(-FMath::Max(0.0f, SanityDamage));
    }
}

void UFearSanityManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    if (!Status || !GetOwner()) return;

    AArenagameCharacter* Player = Cast<AArenagameCharacter>(GetOwner());
    // Unreal does not expose a universal real-time illumination query. Use flashlight state as a
    // conservative default proxy, with a Blueprint override for authored light volumes/sensors.
    const float AutomaticDarkness = Player && !Player->IsFlashlightEnabled() ? 0.35f : 0.0f;
    const float EffectiveDarkness = bHasDarknessOverride ? DarknessExposure : AutomaticDarkness;

    float NearestDistance = EntityDetectionRadius;
    bEntityVisible = false;
    for (TActorIterator<AStalkerEntity> It(GetWorld()); It; ++It)
    {
        AStalkerEntity* Entity = *It;
        const float Distance = FVector::Distance(GetOwner()->GetActorLocation(), Entity->GetActorLocation());
        if (Distance > EntityDetectionRadius) continue;
        NearestDistance = FMath::Min(NearestDistance, Distance);
        FHitResult Hit;
        FCollisionQueryParams Params(SCENE_QUERY_STAT(FearEntitySight), false, GetOwner());
        const bool bBlocked = GetWorld()->LineTraceSingleByChannel(Hit, GetOwner()->GetActorLocation(), Entity->GetActorLocation(), ECC_Visibility, Params);
        if (!bBlocked || Hit.GetActor() == Entity) bEntityVisible = true;
    }
    EntityProximity01 = FMath::Clamp(1.0f - NearestDistance / EntityDetectionRadius, 0.0f, 1.0f);

    const float VisibilityFearScale = bEntityVisible ? 1.0f : 0.35f;
    const float FearRate = EffectiveDarkness * DarknessFearPerSecond + EntityProximity01 * EntityFearPerSecond * VisibilityFearScale;
    if (FearRate > 0.0f)
    {
        Status->ApplyFearDelta(FearRate * DeltaTime);
        if (EntityProximity01 > 0.1f) Status->ApplySanityDelta(-SanityLossNearEntityPerSecond * EntityProximity01 * DeltaTime);
    }
    else
    {
        Status->ApplyFearDelta(-RecoveryPerSecond * DeltaTime);
        Status->ApplySanityDelta(0.5f * DeltaTime);
    }
}
