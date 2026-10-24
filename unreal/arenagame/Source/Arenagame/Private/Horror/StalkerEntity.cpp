#include "Horror/StalkerEntity.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AStalkerEntity::AStalkerEntity()
{
    PrimaryActorTick.bCanEverTick = false;
    AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
    AIControllerClass = AAIController::StaticClass();
    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
}

void AStalkerEntity::BeginPlay()
{
    Super::BeginPlay();
    CachedAIController = Cast<AAIController>(GetController());
    GetWorldTimerManager().SetTimer(AIUpdateTimer, this, &AStalkerEntity::UpdateAI, 0.5f, true);
}

bool AStalkerEntity::CanSeePlayer(ACharacter* Player) const
{
    if (!Player) return false;
    const FVector ToPlayer = Player->GetActorLocation() - GetActorLocation();
    if (ToPlayer.SizeSquared() > FMath::Square(SightRange)) return false;
    if (FVector::DotProduct(GetActorForwardVector(), ToPlayer.GetSafeNormal()) < FMath::Cos(FMath::DegreesToRadians(SightHalfAngle))) return false;
    FHitResult Hit;
    FCollisionQueryParams Params(SCENE_QUERY_STAT(StalkerSight), false, this);
    const FVector Eye = GetActorLocation() + FVector(0.0f, 0.0f, 60.0f);
    return !GetWorld()->LineTraceSingleByChannel(Hit, Eye, Player->GetActorLocation(), ECC_Visibility, Params) || Hit.GetActor() == Player;
}

void AStalkerEntity::UpdateAI()
{
    if (!CachedAIController) CachedAIController = Cast<AAIController>(GetController());
    if (!CachedAIController) return;

    PatrolPoints.RemoveAll([](const TObjectPtr<AActor>& Point) { return !IsValid(Point); });
    if (PatrolPoints.IsEmpty()) PatrolIndex = INDEX_NONE;
    else if (PatrolIndex >= PatrolPoints.Num()) PatrolIndex = INDEX_NONE;

    ACharacter* Player = UGameplayStatics::GetPlayerCharacter(this, 0);
    bHasPlayerSight = Player && CanSeePlayer(Player);
    if (bHasPlayerSight)
    {
        LastSeenPlayerTime = GetWorld()->GetTimeSeconds();
        LastKnownPlayerLocation = Player->GetActorLocation();
        SetState(EStalkerState::Chasing);
        GetCharacterMovement()->MaxWalkSpeed = ChaseSpeed;
        CachedAIController->MoveToLocation(LastKnownPlayerLocation, 100.0f);
        return;
    }

    if (CurrentState == EStalkerState::Chasing)
    {
        if (GetWorld()->GetTimeSeconds() - LastSeenPlayerTime < LosePlayerDelay)
        {
            CachedAIController->MoveToLocation(LastKnownPlayerLocation, 120.0f);
            return;
        }
        SetState(EStalkerState::Patrolling);
    }
    else if (CurrentState == EStalkerState::Investigating)
    {
        if (FVector::DistSquared(GetActorLocation(), InvestigateLocation) < FMath::Square(130.0f))
        {
            SetState(EStalkerState::Patrolling);
        }
        else
        {
            CachedAIController->MoveToLocation(InvestigateLocation, 100.0f);
            return;
        }
    }

    GetCharacterMovement()->MaxWalkSpeed = PatrolSpeed;
    if (PatrolPoints.Num() == 0) return;
    if (PatrolIndex == INDEX_NONE || FVector::DistSquared(GetActorLocation(), PatrolPoints[PatrolIndex]->GetActorLocation()) < FMath::Square(120.0f))
    {
        MoveToNextPatrolPoint();
    }
}

void AStalkerEntity::MoveToNextPatrolPoint()
{
    PatrolPoints.RemoveAll([](const TObjectPtr<AActor>& Point) { return !IsValid(Point); });
    if (PatrolPoints.IsEmpty() || !CachedAIController) return;
    PatrolIndex = PatrolIndex < 0 ? 0 : (PatrolIndex + 1) % PatrolPoints.Num();
    CachedAIController->MoveToActor(PatrolPoints[PatrolIndex], 100.0f);
}

void AStalkerEntity::SetState(EStalkerState NewState)
{
    if (CurrentState == NewState) return;
    CurrentState = NewState;
    OnStalkerStateChanged(NewState);
}

void AStalkerEntity::ReportNoise(FVector NoiseLocation, float Loudness)
{
    if (CurrentState == EStalkerState::Chasing || Loudness <= 0.0f) return;
    InvestigateLocation = NoiseLocation;
    SetState(EStalkerState::Investigating);
    if (!CachedAIController) CachedAIController = Cast<AAIController>(GetController());
    if (CachedAIController) CachedAIController->MoveToLocation(InvestigateLocation, 100.0f);
}
