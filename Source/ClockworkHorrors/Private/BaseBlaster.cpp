// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "BaseBlaster.h"
#include "Components/BoxComponent.h"
#include "Projectile.h"
#include "WeaponPickup.h"
#include "Components/InputComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/PlayerController.h"
#include "InputCoreTypes.h"
#include "Engine/World.h"
#include "CollisionQueryParams.h"
#include "Utils/InventoryItemDataAsset.h"
#include "Components/WidgetComponent.h"
#include "Components/SphereComponent.h"
#include <BaseCharacter.h>
#include "Utils/InventoryComponent.h"
#include "UI/InventorySlotWidget.h"
#include "UI/MainInventoryWidget.h"

// Sets default values
ABaseBlaster::ABaseBlaster()
{
    AnimationStance = ECharacterAnimationStance::Rifle;

    // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = false;
    ActionHappening = false;

    InteractRangeSphere->SetupAttachment(skeletalMesh);
    InteractWidget->SetupAttachment(skeletalMesh);

}

// Called when the game starts or when spawned
void ABaseBlaster::BeginPlay()
{
    Super::BeginPlay();
    //upcast
    AActor* Actor = this->GetParentActor();
    // downcast
    ParentPawn = Cast<APawn>(Actor);
    if (!ParentPawn)
    {

    }
    currentAmmo = -1;


}

void ABaseBlaster::Attack()
{
    ABaseWeapon::Attack();

    if (!CanAttack())
    {
        return;
    }

    if (!ParentPawn || !skeletalMesh || !projectileClass)
    {
        UE_LOG(
            LogTemp,
            Warning,
            TEXT("Blaster cannot fire: ParentPawn, skeletalMesh, or projectileClass is missing.")
        );
        return;
    }

    APlayerController* PlayerController =
        Cast<APlayerController>(ParentPawn->GetController());

    if (!PlayerController)
    {
        UE_LOG(LogTemp, Warning, TEXT("Blaster cannot fire: PlayerController is null."));
        return;
    }

    FVector CameraLocation;
    FRotator CameraRotation;

    PlayerController->GetPlayerViewPoint(
        CameraLocation,
        CameraRotation
    );

    constexpr float TraceDistance = 100000.0f;

    const FVector CameraTraceEnd =
        CameraLocation +
        (CameraRotation.Vector() * TraceDistance);

    FHitResult CameraHit;

    FCollisionQueryParams TraceParams;
    TraceParams.AddIgnoredActor(ParentPawn);
    TraceParams.AddIgnoredActor(this);

    const bool bCameraHit = GetWorld()->LineTraceSingleByChannel(
        CameraHit,
        CameraLocation,
        CameraTraceEnd,
        ECC_Visibility,
        TraceParams
    );

    const FVector AimPoint =
        bCameraHit ? CameraHit.ImpactPoint : CameraTraceEnd;

    const FVector MuzzleLocation =
        skeletalMesh->GetSocketLocation(FName("MuzzleSocket"));

    const FRotator ShootRotation =
        (AimPoint - MuzzleLocation).Rotation();

    FActorSpawnParameters Params;
    Params.Instigator = Cast<APawn>(this);
    Params.Owner = ParentPawn;

    AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
        projectileClass,
        MuzzleLocation,
        ShootRotation,
        Params
    );

    projectile = Cast<AProjectile>(SpawnedActor);

    if (SpawnedActor)
    {
        FTimerHandle ShootTimer;

        ActionHappening = true;

        GetWorld()->GetTimerManager().SetTimer(
            ShootTimer,
            this,
            &ABaseBlaster::ActionStopped,
            1.0f
        );

        OnDelegateInstance.Broadcast();
        useAmmo();
    }
}


const bool ABaseBlaster::CanAttack()
{
    FInventorySlotEntry slot = GetWeaponSlot();
    if(slot.IsValidEntry())
        return !ActionHappening && Alive && slot.Ammo > 0;
    else
    {
        return false;
    }
}

void ABaseBlaster::useAmmo()
{
    UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>();
        ABaseCharacter* player = Cast<ABaseCharacter>(ParentPawn);
        FInventorySlotEntry slot = GetWeaponSlot();
        slot.Ammo -= 1;
        currentAmmo = slot.Ammo;
        player->InventoryComponent->ChangeItemValue(slot, slot.CurrentBind);
        if(slot.IsValidEntry())
        player->OnAmmoChanged.Broadcast(slot.Ammo, MaxAmmo,true);
        else
        {
            player->OnAmmoChanged.Broadcast(0, MaxAmmo,true);
        }

}





void ABaseBlaster::reloadAmmo()
{
    if (currentAmmo < MaxAmmo)
    {
        UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>();
        ABaseCharacter* player = Cast<ABaseCharacter>(ParentPawn);
        FInventorySlotEntry slot = GetAmmoSlot();
        FInventorySlotEntry weaponSlot = GetWeaponSlot();
        if (slot.IsValidEntry())
        {
            if (slot.ItemData->Quantity >= MaxAmmo)
            {
                player->InventoryComponent->RemoveItemsByAmount(slot.ItemData->ItemName,MaxAmmo - weaponSlot.Ammo);
                weaponSlot.Ammo = MaxAmmo;
            }  
            else
            {
                player->InventoryComponent->RemoveItemsByAmount(slot.ItemData->ItemName, slot.ItemData->Quantity);
                weaponSlot.Ammo = slot.ItemData->Quantity;
            }
            player->InventoryComponent->ChangeItemValue(weaponSlot, weaponSlot.CurrentBind);
            currentAmmo = weaponSlot.Ammo;
            player->OnAmmoChanged.Broadcast(currentAmmo, MaxAmmo, true);
        }
    }

    
    
}




FInventorySlotEntry ABaseBlaster::GetWeaponSlot()
{
    if (UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>())
    {
        ABaseCharacter* player = Cast<ABaseCharacter>(ParentPawn);
        if (player)
        {
            FInventorySlotEntry slot = player->InventoryComponent->GetItem(Pickup->ItemDataAsset->ItemName);
            if (slot.IsValidEntry())
            {
                return slot;
            }
        }

    }
    return FInventorySlotEntry();
}

FInventorySlotEntry ABaseBlaster::GetAmmoSlot()
{
    if (UWeaponPickup* Pickup = FindComponentByClass<UWeaponPickup>())
    {
        ABaseCharacter* player = Cast<ABaseCharacter>(ParentPawn);
        if (player)
        {
        FInventorySlotEntry slot = player->InventoryComponent->GetItem(Pickup->ItemDataAsset->AmmoType->ItemName);
        if (slot.IsValidEntry())
        {
            return slot;
        }
        }

    }
    return FInventorySlotEntry();
}
