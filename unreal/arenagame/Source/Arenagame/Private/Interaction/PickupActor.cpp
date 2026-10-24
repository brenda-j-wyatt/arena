#include "Interaction/PickupActor.h"
#include "Objectives/ObjectiveManager.h"
#include "Kismet/GameplayStatics.h"

APickupActor::APickupActor()
{
    InteractionPrompt = NSLOCTEXT("Arenagame", "PickupPrompt", "Pick up");
}

void APickupActor::Interact_Implementation(AActor* InteractingActor)
{
    AObjectiveManager* Manager = ObjectiveManager;
    if (!Manager) Manager = Cast<AObjectiveManager>(UGameplayStatics::GetActorOfClass(this, AObjectiveManager::StaticClass()));
    if (Manager)
    {
        if (ObjectiveItem == EPickupObjectiveItem::FuelCanister) Manager->SetFuelFound();
        else Manager->SetKeycardFound();
    }
    Destroy();
}
