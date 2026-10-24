#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FearSanityManagerComponent.generated.h"

class USurvivalStatusComponent;

UCLASS(ClassGroup=(Arenagame), meta=(BlueprintSpawnableComponent))
class ARENAGAME_API UFearSanityManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFearSanityManagerComponent();
    virtual void BeginPlay() override;
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear", meta=(ClampMin="100.0"))
    float EntityDetectionRadius = 1800.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear")
    float DarknessFearPerSecond = 5.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear")
    float EntityFearPerSecond = 12.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear")
    float RecoveryPerSecond = 4.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Fear")
    float SanityLossNearEntityPerSecond = 3.0f;

    UFUNCTION(BlueprintCallable, Category="Fear")
    void SetDarknessExposure(float Exposure);
    UFUNCTION(BlueprintCallable, Category="Fear")
    void ClearDarknessExposureOverride();
    UFUNCTION(BlueprintCallable, Category="Fear")
    void ReportScriptedScare(float FearAmount, float SanityDamage = 0.0f);
    UFUNCTION(BlueprintPure, Category="Fear")
    bool IsEntityVisible() const { return bEntityVisible; }
    UFUNCTION(BlueprintPure, Category="Fear")
    float GetEntityProximity01() const { return EntityProximity01; }

private:
    UPROPERTY(Transient)
    TObjectPtr<USurvivalStatusComponent> Status;
    float DarknessExposure = 0.0f;
    bool bHasDarknessOverride = false;
    float EntityProximity01 = 0.0f;
    bool bEntityVisible = false;
};
