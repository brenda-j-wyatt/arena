#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interaction/InteractionInterface.h"
#include "InteractableActor.generated.h"

class UStaticMeshComponent;

UCLASS(Blueprintable)
class ARENAGAME_API AInteractableActor : public AActor, public IInteractionInterface
{
    GENERATED_BODY()

public:
    AInteractableActor();
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
    TObjectPtr<UStaticMeshComponent> Mesh;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interaction")
    FText InteractionPrompt = NSLOCTEXT("Arenagame", "InteractPrompt", "Interact");

    virtual void Interact_Implementation(AActor* InteractingActor) override;
    virtual FText GetInteractionText_Implementation() const override;
};
