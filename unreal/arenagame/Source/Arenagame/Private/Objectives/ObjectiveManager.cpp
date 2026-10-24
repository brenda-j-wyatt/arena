#include "Objectives/ObjectiveManager.h"

AObjectiveManager::AObjectiveManager()
{
    PrimaryActorTick.bCanEverTick = false;
}

void AObjectiveManager::BroadcastIfChanged(bool bChanged)
{
    if (bChanged) OnObjectiveUpdated.Broadcast();
}
void AObjectiveManager::SetFuelFound(bool bFound) { const bool bChanged = bFuelFound != bFound; bFuelFound = bFound; BroadcastIfChanged(bChanged); }
void AObjectiveManager::SetKeycardFound(bool bFound) { const bool bChanged = bKeycardFound != bFound; bKeycardFound = bFound; BroadcastIfChanged(bChanged); }
void AObjectiveManager::SetPowerRestored(bool bRestored) { const bool bChanged = bPowerRestored != bRestored; bPowerRestored = bRestored; BroadcastIfChanged(bChanged); }
void AObjectiveManager::SetEscapeReached(bool bReached) { const bool bChanged = bEscapeReached != bReached; bEscapeReached = bReached; BroadcastIfChanged(bChanged); }

FText AObjectiveManager::GetCurrentObjective() const
{
    if (!bFuelFound) return NSLOCTEXT("Arenagame", "ObjectiveFuel", "Find a fuel canister");
    if (!bKeycardFound) return NSLOCTEXT("Arenagame", "ObjectiveKeycard", "Find the keycard");
    if (!bPowerRestored) return NSLOCTEXT("Arenagame", "ObjectivePower", "Restore power");
    if (!bEscapeReached) return NSLOCTEXT("Arenagame", "ObjectiveEscape", "Reach the boat escape point");
    return NSLOCTEXT("Arenagame", "ObjectiveComplete", "Escape the station");
}
