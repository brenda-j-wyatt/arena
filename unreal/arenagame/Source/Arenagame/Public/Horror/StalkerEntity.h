#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "StalkerEntity.generated.h"

class AAIController;

UENUM(BlueprintType)
enum class EStalkerState : uint8
{
    Patrolling,
    Investigating,
    Chasing
};

UCLASS(Blueprintable)
class ARENAGAME_API AStalkerEntity : public ACharacter
{
    GENERATED_BODY()

public:
    AStalkerEntity();
    virtual void BeginPlay() override;

    UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI|Patrol")
    TArray<TObjectPtr<AActor>> PatrolPoints;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Senses", meta=(ClampMin="100.0"))
    float SightRange = 2200.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Senses", meta=(ClampMin="0.0", ClampMax="180.0"))
    float SightHalfAngle = 65.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Movement")
    float PatrolSpeed = 220.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Movement")
    float ChaseSpeed = 430.0f;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="AI|Search")
    float LosePlayerDelay = 5.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="AI")
    EStalkerState CurrentState = EStalkerState::Patrolling;

    UFUNCTION(BlueprintCallable, Category="AI|Noise")
    void ReportNoise(FVector NoiseLocation, float Loudness = 1.0f);
    UFUNCTION(BlueprintPure, Category="AI")
    bool HasLineOfSightToPlayer() const { return bHasPlayerSight; }

protected:
    UFUNCTION(BlueprintImplementableEvent, Category="AI")
    void OnStalkerStateChanged(EStalkerState NewState);

private:
    void UpdateAI();
    void SetState(EStalkerState NewState);
    void MoveToNextPatrolPoint();
    bool CanSeePlayer(ACharacter* Player) const;

    UPROPERTY(Transient)
    TObjectPtr<AAIController> CachedAIController;
    int32 PatrolIndex = INDEX_NONE;
    float LastSeenPlayerTime = -10000.0f;
    bool bHasPlayerSight = false;
    FTimerHandle AIUpdateTimer;
    FVector LastKnownPlayerLocation = FVector::ZeroVector;
    FVector InvestigateLocation = FVector::ZeroVector;
};
