#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StormLightingController.generated.h"

class ADirectionalLight;
class AExponentialHeightFog;

UCLASS(Blueprintable)
class ARENAGAME_API AStormLightingController : public AActor
{
    GENERATED_BODY()

public:
    AStormLightingController();
    virtual void Tick(float DeltaSeconds) override;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Storm")
    TObjectPtr<ADirectionalLight> DirectionalLight;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Storm")
    TObjectPtr<AExponentialHeightFog> ExponentialFog;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Storm", meta=(ClampMin="0.0"))
    float StormLightIntensity = 0.15f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Storm", meta=(ClampMin="0.0"))
    float StormFogDensity = 0.035f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Storm")
    float StormPulseAmount = 0.025f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Storm", meta=(ClampMin="0.0"))
    float StormPulseSpeed = 0.22f;
    UFUNCTION(BlueprintCallable, Category="Storm")
    void SetStormIntensity(float NormalizedIntensity);

private:
    float ElapsedTime = 0.0f;
    float IntensityScale = 1.0f;
};
