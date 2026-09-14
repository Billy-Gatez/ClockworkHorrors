// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "ClockworkPlayerController.h"
#include "TargetLockTestPlayerController.generated.h"

/**
 * Deprecated compatibility shell for the original isolated Z-lock test map.
 *
 * Target-lock input now lives in AClockworkPlayerController. Keeping this
 * class temporarily prevents BP_ZLockTestPlayerController from losing its
 * native parent while the production migration is validated.
 *
 * Once all production maps are verified, the test Blueprint can be reparented
 * to AClockworkPlayerController and this class can be removed.
 */
UCLASS()
class CLOCKWORKHORRORS_API ATargetLockTestPlayerController : public AClockworkPlayerController
{
    GENERATED_BODY()
};
