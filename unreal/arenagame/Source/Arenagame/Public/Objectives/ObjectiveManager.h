#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectiveManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnObjectiveUpdated);

UCLASS(Blueprintable)
class ARENAGAME_API AObjectiveManager : public AActor
{
    GENERATED_BODY()

public:
    AObjectiveManager();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives")
    bool bFuelFound = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives")
    bool bKeycardFound = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives")
    bool bPowerRestored = false;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Objectives")
    bool bEscapeReached = false;
    UPROPERTY(BlueprintAssignable, Category="Objectives")
    FOnObjectiveUpdated OnObjectiveUpdated;

    UFUNCTION(BlueprintCallable, Category="Objectives")
    void SetFuelFound(bool bFound = true);
    UFUNCTION(BlueprintCallable, Category="Objectives")
    void SetKeycardFound(bool bFound = true);
    UFUNCTION(BlueprintCallable, Category="Objectives")
    void SetPowerRestored(bool bRestored = true);
    UFUNCTION(BlueprintCallable, Category="Objectives")
    void SetEscapeReached(bool bReached = true);
    UFUNCTION(BlueprintPure, Category="Objectives")
    FText GetCurrentObjective() const;

private:
    void BroadcastIfChanged(bool bChanged);
};
