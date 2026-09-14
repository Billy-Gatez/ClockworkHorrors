// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TargetableComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnTargetedStateChanged,
    bool, bIsTargeted
);

/**
 * Marks an actor as a valid lock-on target.
 *
 * AEnemy owns this component in the production setup, so all enemy children
 * inherit the same targeting behavior. It can also be added to any other actor
 * that should participate in the lock-on system.
 */
UCLASS(ClassGroup = (Targeting), meta = (BlueprintSpawnableComponent))
class CLOCKWORKHORRORS_API UTargetableComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UTargetableComponent();

    /** Can this actor currently be selected/kept as a lock-on target? */
    UFUNCTION(BlueprintPure, Category = "Targeting")
    bool CanBeTargeted() const { return bCanBeTargeted; }

    /** Enable/disable targeting at runtime (useful for death, cutscenes, etc.). */
    UFUNCTION(BlueprintCallable, Category = "Targeting")
    void SetCanBeTargeted(bool bNewCanBeTargeted);

    /** World-space point the camera/lock system should aim toward. */
    UFUNCTION(BlueprintPure, Category = "Targeting")
    FVector GetTargetLocation() const;

    /** Extra score added when choosing between otherwise valid targets. */
    UFUNCTION(BlueprintPure, Category = "Targeting")
    float GetTargetPriority() const { return TargetPriority; }

    /** Is this actor the player's currently selected lock-on target? */
    UFUNCTION(BlueprintPure, Category = "Targeting")
    bool IsTargeted() const { return bIsTargeted; }

    /**
     * Update the presentation-facing targeted state.
     * TargetLockComponent calls this automatically when locking, switching,
     * or releasing a target. Gameplay/UI can listen to OnTargetedStateChanged.
     */
    UFUNCTION(BlueprintCallable, Category = "Targeting")
    void SetTargeted(bool bNewTargeted);

    /**
     * Fired only when the targeted state actually changes.
     * This is intentionally presentation-agnostic: enemy Blueprints can show
     * a widget, outline, material effect, sound, etc. without the player
     * targeting code casting to a specific enemy class.
     */
    UPROPERTY(BlueprintAssignable, Category = "Targeting")
    FOnTargetedStateChanged OnTargetedStateChanged;

protected:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
    bool bCanBeTargeted = true;

    /** Runtime state used by presentation systems; not a designer-authored default. */
    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Targeting")
    bool bIsTargeted = false;

    /**
     * Local-space offset from the owning actor's location.
     * For a normal humanoid enemy, Z = 80-120 is a useful starting point.
     */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
    FVector TargetOffset = FVector(0.0f, 0.0f, 90.0f);

    /** Higher values make this target more desirable during initial selection. */
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Targeting")
    float TargetPriority = 0.0f;
};
