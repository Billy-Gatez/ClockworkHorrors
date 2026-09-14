// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InputActionValue.h"
#include "ClockworkPlayerController.generated.h"

class UInputAction;
class UInputMappingContext;
class UTargetLockComponent;

UCLASS()
class CLOCKWORKHORRORS_API AClockworkPlayerController : public APlayerController
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
    virtual void SetupInputComponent() override;

    void Move(const FInputActionValue& Value);
    void Look(const FInputActionValue& Value);
    void MouseLook(const FInputActionValue& Value);
    void StartJump();
    void StopJump();
    void Interact();
    void Attack();
    void StopAttack();
    void Reload();
    void OpenInventory();
    void KillPlayer();

    void Slot1();
    void Slot2();
    void Slot3();
    void Slot4();
    void Slot5();
    void Slot6();
    void Slot7();
    void Slot8();
    void Slot9();
    void Slot10();
    void DropWeapon();
    void ClearInventory();
    void LevelUpButton();
private:
    void HandlePauseInput();
    void HandleQuitInput();

    // Target-lock input handlers. The controller remains usable by Pawns that
    // do not have a TargetLockComponent; these simply no-op in that case.
    void ToggleTargetLock();
    void HandleTargetSwitch(const FInputActionValue& Value);
    void HandleLockedCameraLook(const FInputActionValue& Value);
    void HandleLockedCameraMouseLook(const FInputActionValue& Value);
    UTargetLockComponent* GetTargetLockComponent() const;

public:
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputMappingContext* DefaultMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MoveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* LookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* MouseLookAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* JumpAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* InteractAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ComboAttackAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ReloadAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* PauseAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* QuitGameAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* InventoryAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* KillAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotOneAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotTwoAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotThreeAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotFourAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotFiveAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotSixAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotSevenAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotEightAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotNineAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* WeaponSlotTenAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* DropWeaponAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
    UInputAction* ClearInventoryAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    UInputAction* DebugLevelUpAction;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float MouseSensitivity = 45.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
    float LookSensitivity = 100.0f;

    // =========================================================
    // TARGET LOCK INPUT
    // =========================================================

    /** Separate mapping context for target lock/switch bindings. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Target Lock")
    UInputMappingContext* TargetLockMappingContext;

    /** Digital lock toggle action. Current prototype uses Q and R3. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Target Lock")
    UInputAction* TargetLockAction;

    /** Axis1D switch action. Negative = left, positive = right. */
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Target Lock")
    UInputAction* TargetSwitchAction;
};
