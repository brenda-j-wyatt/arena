#include "Atmosphere/AmbientAudioController.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"

AAmbientAudioController::AAmbientAudioController()
{
    PrimaryActorTick.bCanEverTick = false;
}

UAudioComponent* AAmbientAudioController::SpawnLoop(USoundBase* Sound, float Volume)
{
    if (!Sound) return nullptr;
    UAudioComponent* Audio = UGameplayStatics::SpawnSound2D(this, Sound, FMath::Max(0.0f, Volume));
    if (Audio) Audio->bIsUISound = false;
    return Audio;
}

void AAmbientAudioController::PlayWind(float Volume)
{
    OnWindRequested();
    if (WindAudio) WindAudio->Stop();
    WindAudio = SpawnLoop(WindSound, Volume);
}
void AAmbientAudioController::PlayHeartbeat(float Volume)
{
    OnHeartbeatRequested(Volume);
    if (HeartbeatAudio) HeartbeatAudio->Stop();
    HeartbeatAudio = SpawnLoop(HeartbeatSound, Volume);
}
void AAmbientAudioController::SetHeartbeatVolume(float Volume)
{
    OnHeartbeatRequested(Volume);
    if (HeartbeatAudio) HeartbeatAudio->SetVolumeMultiplier(FMath::Clamp(Volume, 0.0f, 1.0f));
    else HeartbeatAudio = SpawnLoop(HeartbeatSound, Volume);
}
void AAmbientAudioController::PlayEntityStinger(float Volume)
{
    OnEntityStingerRequested();
    if (EntityStingerSound) UGameplayStatics::SpawnSound2D(this, EntityStingerSound, Volume);
}
void AAmbientAudioController::PlayRadioStatic(float Volume)
{
    OnRadioStaticRequested();
    if (RadioStaticSound) UGameplayStatics::SpawnSound2D(this, RadioStaticSound, Volume);
}
void AAmbientAudioController::StopAmbientAudio()
{
    if (WindAudio) WindAudio->Stop();
    if (HeartbeatAudio) HeartbeatAudio->Stop();
}
