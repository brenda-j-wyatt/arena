#pragma once

#include "CoreMinimal.h"
#include "Interaction/InteractableActor.h"
#include "PickupActor.generated.h"

class AObjectiveManager;

UENUM(BlueprintType)
enum class EPickupObjectiveItem : uint8
{
    FuelCanister,
    Keycard
};

UCLASS(Blueprintable)
class ARENAGAME_API APickupActor : public AInteractableActor
{
    GENERATED_BODY()

public:
    APickupActor();
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Pickup")
    EPickupObjectiveItem ObjectiveItem = EPickupObjectiveItem::FuelCanister;
    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="Pickup")
    TObjectPtr<AObjectiveManager> ObjectiveManager;
    virtual void Interact_Implementation(AActor* InteractingActor) override;
};
