// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "Animation/CharacterAnimationSet.h"
#include "Spells.h"
#include "Staff.generated.h"

class ACharacter;
class ASpellProjectile;
class USceneComponent;
class UStaticMeshComponent;
class UWeaponPickup;
class UManaComponent;
class USpellSelectorWidget;

UCLASS(Blueprintable)
class CLOCKWORKHORRORS_API AStaff : public ABaseWeapon
{
    GENERATED_BODY()

public:
    AStaff();


    virtual void BeginPlay() override;

    virtual void Attack() override;

    UFUNCTION(BlueprintCallable, Category = "Staff|Casting")
    void ReleaseSpellCharge();

    UFUNCTION(BlueprintCallable, Category = "Staff|Casting")
    bool AttemptSpellCast(AActor* CastingActor, FVector CastingDirection);

    UFUNCTION(BlueprintPure, Category = "Staff|Casting")
    bool IsSpellReady() const;

    UFUNCTION(BlueprintPure, Category = "Staff|Casting")
    bool IsChargingSpell() const;

    UFUNCTION(BlueprintPure, Category = "Staff|Casting")
    float GetCurrentChargePercent() const;

    UFUNCTION(BlueprintPure, Category = "Staff|Mana")
    float GetSelectedSpellManaCost() const;

    UFUNCTION(BlueprintPure, Category = "Staff|Mana")
    bool HasEnoughManaForSelectedSpell() const;

    UFUNCTION(BlueprintPure, Category = "Staff|Spell")
    ASpells* GetSelectedSpell() const;

    UFUNCTION(BlueprintPure, Category = "Staff|Spell")
    int32 GetSelectedSpellSlot() const;

    UFUNCTION(BlueprintCallable, Category = "Staff|Spell")
    void SelectNextSpell();

    UFUNCTION(BlueprintCallable, Category = "Staff|Spell")
    void SelectPreviousSpell();

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Staff|Components")
    TObjectPtr<UStaticMeshComponent> StaffAppearance;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Staff|Components")
    TObjectPtr<USceneComponent> SpellReleasePoint;

    UPROPERTY(
        EditAnywhere,
        BlueprintReadWrite,
        Category = "Staff|Spell",
        meta = (EditFixedSize)
    )
    TArray<TSubclassOf<ASpells>> SpellSlots;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Staff|UI")
    TSubclassOf<USpellSelectorWidget> SpellSelectorWidgetClass;

protected:
    UFUNCTION(BlueprintImplementableEvent, Category = "Staff|Casting")
    void SpellCastRequested(
        AActor* CastingActor,
        FVector CastingDirection,
        ASpells* SelectedSpell
    );

private:
    void SetupStaffInput();
    void UpdateStaffInputConsumption();

    void HandleCastPressed();
    void HandleSpellScrollUp();
    void HandleSpellScrollDown();

    UFUNCTION()
    void HandleStaffWeaponEquippedStateChanged(bool bWeaponEquipped);

    bool IsStaffControlledByPlayer() const;

    bool AttemptSpellCastCharged(AActor* CastingActor, FVector CastingDirection, float ChargePercent);

    bool HandleRedirectableProjectileInput(AActor* CastingActor, const FVector& CastingDirection);
    bool HandleGravityProjectileInput();

    UManaComponent* GetManaComponent() const;

    void CreateSpellSelectorWidget();
    void RemoveSpellSelectorWidget();
    void RefreshSpellSelector(int32 Direction);
    int32 FindAdjacentSpellIndex(int32 StartIndex, int32 Direction) const;
    FText GetSpellNameAtIndex(int32 SpellIndex) const;

    UPROPERTY()
    TObjectPtr<UWeaponPickup> WeaponPickup = nullptr;

    UPROPERTY()
    TObjectPtr<ACharacter> AnimationHolder = nullptr;

    UPROPERTY()
    TObjectPtr<USpellSelectorWidget> SpellSelectorWidget = nullptr;

    TArray<float> SpellCooldownEndTimes;

    bool bStaffInputBindingsCreated = false;
    bool bIsCharging = false;
    bool bRedirectInputLatch = false;
    float ChargeStartTimeSeconds = 0.0f;
    int32 SelectedSpellIndex = 0;
    int32 ActiveRedirectableSpellIndex = INDEX_NONE;
    TWeakObjectPtr<ASpellProjectile> ActiveRedirectableProjectile;
    int32 ActiveGravitySpellIndex = INDEX_NONE;
    TWeakObjectPtr<ASpellProjectile> ActiveGravityProjectile;
};