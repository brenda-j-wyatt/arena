#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractionInterface.generated.h"

UINTERFACE(BlueprintType)
class ARENAGAME_API UInteractionInterface : public UInterface
{
    GENERATED_BODY()
};

class ARENAGAME_API IInteractionInterface
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    void Interact(AActor* InteractingActor);
    UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Interaction")
    FText GetInteractionText() const;
};
