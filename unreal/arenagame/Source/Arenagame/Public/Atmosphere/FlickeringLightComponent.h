#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlickeringLightComponent.generated.h"

class ULightComponent;

UCLASS(ClassGroup=(Arenagame), meta=(BlueprintSpawnableComponent))
class ARENAGAME_API UFlickeringLightComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UFlickeringLightComponent();
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flicker", meta=(ClampMin="0.01"))
    float FlickerInterval = 0.08f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flicker", meta=(ClampMin="0.0", ClampMax="1.0"))
    float FlickerChance = 0.3f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flicker", meta=(ClampMin="0.0", ClampMax="1.0"))
    float MinimumIntensityMultiplier = 0.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Flicker")
    bool bStartEnabled = true;

    UFUNCTION(BlueprintCallable, Category="Flicker")
    void SetFlickering(bool bEnabled);

private:
    void Flicker();
    UPROPERTY(Transient)
    TObjectPtr<ULightComponent> Light;
    FTimerHandle FlickerTimer;
    float OriginalIntensity = 0.0f;
    bool bFlickering = false;
};
