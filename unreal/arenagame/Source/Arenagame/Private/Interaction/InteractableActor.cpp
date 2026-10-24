#include "Interaction/InteractableActor.h"
#include "Components/StaticMeshComponent.h"

AInteractableActor::AInteractableActor()
{
    PrimaryActorTick.bCanEverTick = false;
    Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
    SetRootComponent(Mesh);
}

void AInteractableActor::Interact_Implementation(AActor* InteractingActor) {}
FText AInteractableActor::GetInteractionText_Implementation() const { return InteractionPrompt; }
