#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmbientAudioController.generated.h"

class USoundBase;
class UAudioComponent;

UCLASS(Blueprintable)
class ARENAGAME_API AAmbientAudioController : public AActor
{
    GENERATED_BODY()

public:
    AAmbientAudioController();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    TObjectPtr<USoundBase> WindSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    TObjectPtr<USoundBase> HeartbeatSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    TObjectPtr<USoundBase> EntityStingerSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Audio")
    TObjectPtr<USoundBase> RadioStaticSound;

    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayWind(float Volume = 1.0f);
    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayHeartbeat(float Volume = 1.0f);
    UFUNCTION(BlueprintCallable, Category="Audio")
    void SetHeartbeatVolume(float Volume);
    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayEntityStinger(float Volume = 1.0f);
    UFUNCTION(BlueprintCallable, Category="Audio")
    void PlayRadioStatic(float Volume = 1.0f);
    UFUNCTION(BlueprintCallable, Category="Audio")
    void StopAmbientAudio();

protected:
    UFUNCTION(BlueprintImplementableEvent, Category="Audio|Hooks")
    void OnWindRequested();
    UFUNCTION(BlueprintImplementableEvent, Category="Audio|Hooks")
    void OnHeartbeatRequested(float Volume);
    UFUNCTION(BlueprintImplementableEvent, Category="Audio|Hooks")
    void OnEntityStingerRequested();
    UFUNCTION(BlueprintImplementableEvent, Category="Audio|Hooks")
    void OnRadioStaticRequested();

private:
    UAudioComponent* SpawnLoop(USoundBase* Sound, float Volume);
    UPROPERTY(Transient)
    TObjectPtr<UAudioComponent> WindAudio;
    UPROPERTY(Transient)
    TObjectPtr<UAudioComponent> HeartbeatAudio;
};
