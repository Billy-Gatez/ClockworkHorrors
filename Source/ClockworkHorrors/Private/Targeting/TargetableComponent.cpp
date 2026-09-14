// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#include "Targeting/TargetableComponent.h"

UTargetableComponent::UTargetableComponent()
{
    PrimaryComponentTick.bCanEverTick = false;
}

void UTargetableComponent::SetCanBeTargeted(bool bNewCanBeTargeted)
{
    if (bCanBeTargeted == bNewCanBeTargeted)
    {
        return;
    }

    bCanBeTargeted = bNewCanBeTargeted;

    // Presentation should clear immediately when an actor stops being
    // targetable (death, cutscene state, invulnerability phase, etc.).
    // TargetLockComponent will release the gameplay lock on its next update.
    if (!bCanBeTargeted && bIsTargeted)
    {
        SetTargeted(false);
    }
}

void UTargetableComponent::SetTargeted(bool bNewTargeted)
{
    if (bIsTargeted == bNewTargeted)
    {
        return;
    }

    bIsTargeted = bNewTargeted;
    OnTargetedStateChanged.Broadcast(bIsTargeted);
}

FVector UTargetableComponent::GetTargetLocation() const
{
    const AActor* Owner = GetOwner();
    if (!Owner)
    {
        return FVector::ZeroVector;
    }

    return Owner->GetActorLocation()
        + Owner->GetActorTransform().TransformVectorNoScale(TargetOffset);
}
