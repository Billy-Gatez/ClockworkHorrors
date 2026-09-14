// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.


#include "UI/MainInventoryWidget.h"
#include "BaseCharacter.h"
#include "UI/InventoryGridWidget.h"
#include "UI/ItemActionWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Blueprint/SlateBlueprintLibrary.h"
#include "UI/ButtonWithText.h"
#include "Components/WidgetSwitcher.h"
#include "Components/TextBlock.h"
#include "Utils/ExperienceComponent.h"

UMainInventoryWidget::UMainInventoryWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void UMainInventoryWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if(InventoryGrid)
	{
		InventoryGrid->SetInventoryMainReference(this);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("UMainInventoryWidget::NativeConstruct InventoryGrid is not initialized!"));
	}
	if(ItemActionWidget)
		ItemActionWidget->SetVisibility(ESlateVisibility::Hidden);
	SwordSkillButtons =
	{
		SwordUpgrade1,
		SwordUpgrade2,
		SwordUpgrade3,
		SwordUpgrade4,
		SwordUpgrade5,
		SwordUpgrade6,
		SwordUpgrade7,
		SwordUpgrade8,
		SwordUpgrade9,
		SwordUpgrade10
	};
	StaffSkillButtons =
	{
		StaffUpgrade1,
		StaffUpgrade2,
		StaffUpgrade3,
		StaffUpgrade4,
		StaffUpgrade5,
		StaffUpgrade6,
		StaffUpgrade7,
		StaffUpgrade8,
		StaffUpgrade9,
		StaffUpgrade10
	};
	BlasterSkillButtons =
	{
		BlasterUpgrade1,
		BlasterUpgrade2,
		BlasterUpgrade3,
		BlasterUpgrade4,
		BlasterUpgrade5,
		BlasterUpgrade6,
		BlasterUpgrade7,
		BlasterUpgrade8,
		BlasterUpgrade9,
		BlasterUpgrade10
	};
	if (InventoryButton)
	{
		InventoryButton->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::InventoryButtonClicked);
	}
	if (SkillTreeButton)
	{
		SkillTreeButton->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::SkillTreeButtonClicked);
	}
	if (SwordSkillTreeButton)
	{
		SwordSkillTreeButton->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::SwordSkillTreeButtonClicked);
	}
	if (StaffSkillTreeButton)
	{
		StaffSkillTreeButton->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::StaffSkillTreeButtonClicked);
	}
	if (BlasterSkillTreeButton)
	{
		BlasterSkillTreeButton->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::BlasterSkillTreeButtonClicked);
	}
	//if (SwordUpgrade1)
	//{
	//	SwordUpgrade1->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::SwordSkillOneClicked);
	//}
	//if (StaffUpgrade1)
	//{
	//	StaffUpgrade1->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::StaffSkillOneClicked);
	//}
	//if (BlasterUpgrade1)
	//{
	//	BlasterUpgrade1->InternalButtonClicked.AddDynamic(this, &UMainInventoryWidget::BlasterSkillOneClicked);
	//}
	if (SwordUpgrade1)
	{
		SwordUpgrade1->InternalButtonClickedWithSource.AddUniqueDynamic(
			this,
			&UMainInventoryWidget::HandleSkillButtonClicked
		);
	}

	if (StaffUpgrade1)
	{
		StaffUpgrade1->InternalButtonClickedWithSource.AddUniqueDynamic(
			this,
			&UMainInventoryWidget::HandleSkillButtonClicked
		);
	}

	if (BlasterUpgrade1)
	{
		BlasterUpgrade1->InternalButtonClickedWithSource.AddUniqueDynamic(
			this,
			&UMainInventoryWidget::HandleSkillButtonClicked
		);
	}
	if (SkillPointsText)
	{
		SkillPointsTextUpdated(0);
	}
}

void UMainInventoryWidget::ShowInventory(bool bInventoryShown)
{
	UWorld* World = GetWorld();

	if (World)
	{
		UGameInstance* GameInstance = GetGameInstance();

		if (GameInstance)
		{
			//UE_LOG(LogTemp, Log, TEXT("UMainInventoryWidget::ShowInventory GameInstance found: %s"), *GameInstance->GetName());
			APlayerController* const PC = GameInstance ? GameInstance->GetFirstLocalPlayerController() : nullptr;
			if(!PC)
			{
				UE_LOG(LogTemp, Error, TEXT("UMainInventoryWidget::ShowInventory PlayerController not found!"));
				return;
			}
			if (bInventoryShown)
			{
				SetVisibility(ESlateVisibility::Visible);
				AddToViewport(98);
				FInputModeGameAndUI InputMode;
				InputMode.SetWidgetToFocus(TakeWidget());
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = true;
			}
			else
			{
				SetVisibility(ESlateVisibility::Hidden);
				FInputModeGameOnly InputMode;
				PC->SetInputMode(InputMode);
				PC->bShowMouseCursor = false;
			}
		}
	}
}

void UMainInventoryWidget::HandleSlotPressed(const FInventorySlotEntry& ItemEntry, int32 SlotIndex, FVector2D ScreenPosition)
{
	if(ItemActionWidget)
	{
		FVector2D PixelPosition;
		FVector2D ViewportPosition;
		USlateBlueprintLibrary::AbsoluteToViewport(this, ScreenPosition, PixelPosition, ViewportPosition);

		if (UCanvasPanelSlot* CanvasSlot = Cast<UCanvasPanelSlot>(ItemActionWidget->Slot))
		{
			CanvasSlot->SetPosition(ViewportPosition + FVector2D(20.f, 0.f));
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("ItemActionWidget is not a CanvasPanelSlot!"));
		}

		//UE_LOG(LogTemp, Warning, TEXT("UMainInventoryWidget::HandleSlotPressed: Showing ItemActionWidget for Item: %s at Slot Index: %d"), *ItemEntry.GetItemDataName().ToString(), SlotIndex);
		ItemActionWidget->ShowWidget(ItemEntry, SlotIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemActionWidget is not initialized!"));
	}
}

void UMainInventoryWidget::HandleDragStarted()
{
	if(ItemActionWidget)
	{
		//UE_LOG(LogTemp, Warning, TEXT("UMainInventoryWidget::HandleDragStarted: Hiding ItemActionWidget"));
		ItemActionWidget->HideWidget();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ItemActionWidget is not initialized!"));
	}
}

void UMainInventoryWidget::UpdateInventoryUI(const FInventorySlotEntry& ItemEntry, int32 SlotIndex, bool ItemSwaped)
{
	if(InventoryGrid)
	{
		UE_LOG(LogTemp, Log, TEXT("Updating Inventory UI for Item: %s at Slot Index: %d"), *ItemEntry.GetItemDataName().ToString(), SlotIndex);
		InventoryGrid->AddItemDataToSlot(ItemEntry, SlotIndex, ItemSwaped);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryGrid is not initialized!"));
	}
}

void UMainInventoryWidget::UpdateInventoryUI_Remove(FName ItemName, int32 Quantity, int32 SlotIndex)
{
	if(InventoryGrid)
	{
		//UE_LOG(LogTemp, Log, TEXT("Updating Inventory UI for Item: %s at Slot Index: %d"), *ItemEntry.GetItemDataName().ToString(), SlotIndex);
		InventoryGrid->RemoveItemFromSlot(ItemName, Quantity, SlotIndex);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryGrid is not initialized!"));
	}
}

void UMainInventoryWidget::SetInventoryGridMaxSlotCount(int32 NewSlotCount)
{
	UE_LOG(LogTemp, Error, TEXT("Setting Inventory Grid Max Slot Count to: %d"), NewSlotCount);
	if(InventoryGrid)
	{
		InventoryGrid->SetInventoryMaxSlotCount(NewSlotCount);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("InventoryGrid is not initialized!"));
	}
}

void UMainInventoryWidget::InventoryButtonClicked()
{
	MenuTabSwitcher->SetActiveWidgetIndex(0);
}

void UMainInventoryWidget::SkillTreeButtonClicked()
{
	MenuTabSwitcher->SetActiveWidgetIndex(1);
}

void UMainInventoryWidget::SwordSkillTreeButtonClicked()
{
	SetExperienceComponent();
				if (expComp->GetSkillPoints() > 0)
				{
					expComp->SetSkillPoints(expComp->GetSkillPoints() - 1);
					SkillPointsTextUpdated(expComp->GetSkillPoints());
					PathSwitcher->SetActiveWidgetIndex(1);
					SkillTreePath = 1;
				}

	
}

void UMainInventoryWidget::StaffSkillTreeButtonClicked()
{
	SetExperienceComponent();
	if (expComp->GetSkillPoints() > 0)
	{
		expComp->SetSkillPoints(expComp->GetSkillPoints() - 1);
		SkillPointsTextUpdated(expComp->GetSkillPoints());
		PathSwitcher->SetActiveWidgetIndex(2);
		SkillTreePath = 2;
	}
}

void UMainInventoryWidget::BlasterSkillTreeButtonClicked()
{
	SetExperienceComponent();
	if (expComp->GetSkillPoints() > 0)
	{
		expComp->SetSkillPoints(expComp->GetSkillPoints() - 1);
		SkillPointsTextUpdated(expComp->GetSkillPoints());
		PathSwitcher->SetActiveWidgetIndex(3);
		SkillTreePath = 3;
	}
}



void UMainInventoryWidget::SkillPointsTextUpdated(int points)
{
	
	SkillPointsText->SetText(FText::FromString(FString::Printf(TEXT("SkillPoints: %d"), points)));
}

void UMainInventoryWidget::SetExperienceComponent()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController) {
		ABaseCharacter* player = Cast<ABaseCharacter>(PlayerController->GetPawn());
		if (player)
		{
			if (UExperienceComponent* exp = player->GetComponentByClass<UExperienceComponent>())
			{
				expComp = exp;
			}
		}
	}

}

void UMainInventoryWidget::LoadSkillTree(int32 SkillPath, FString skillTreeSelection)
{
	loadingData = true;
	SkillTreePath = SkillPath;
	skillSelection = skillTreeSelection;
	PathSwitcher->SetActiveWidgetIndex(SkillTreePath);
	switch (SkillTreePath)
	{
	case 1:
	{
		for (size_t i = 0; i < skillSelection.Len(); i++)
		{
			if (skillSelection[i] == '1') {
				HandleSkillButtonClicked(SwordSkillButtons[i]);
			}
		}
		break;
	}
	case 2:
	{
		for (size_t i = 0; i < skillSelection.Len(); i++)
		{
			if (skillSelection[i] == '1') {
				HandleSkillButtonClicked(StaffSkillButtons[i]);
			}
		}
		break;
	}
	case 3:
	{
		for (size_t i = 0; i < skillSelection.Len(); i++)
		{
			if (skillSelection[i] == '1') {
				HandleSkillButtonClicked(BlasterSkillButtons[i]);
			}
		}
		break;
	}
	default:
		break;
	}
	loadingData = false;
}

void UMainInventoryWidget::HandleSkillButtonClicked(UButtonWithText* ClickedButton)
{
	if (!ClickedButton)
	{
		return;
	}

	TArray<UButtonWithText*>* ActiveSkillButtons = nullptr;
	const TCHAR* SkillPathName = TEXT("Unknown");

	if (SwordSkillButtons.Contains(ClickedButton))
	{
		ActiveSkillButtons = &SwordSkillButtons;
		SkillPathName = TEXT("Sword");
	}
	else if (StaffSkillButtons.Contains(ClickedButton))
	{
		ActiveSkillButtons = &StaffSkillButtons;
		SkillPathName = TEXT("Staff");
	}
	else if (BlasterSkillButtons.Contains(ClickedButton))
	{
		ActiveSkillButtons = &BlasterSkillButtons;
		SkillPathName = TEXT("Blaster");
	}

	if (!ActiveSkillButtons)
	{
		return;
	}
	const int32 SkillIndex = ActiveSkillButtons->Find(ClickedButton);
	skillSelection[SkillIndex] = '1';
	if (!expComp)
	{
		SetExperienceComponent();
	}
	if (!loadingData)
	{
		if(expComp->GetSkillPoints() > 0)
			expComp->SetSkillPoints(expComp->GetSkillPoints() - 1);
		else
		{
			return;
		}
	}
	OnUpgradeSkill.Broadcast(SkillTreePath, SkillIndex);

	SkillPointsTextUpdated(expComp->GetSkillPoints());

	UE_LOG(
		LogTemp,
		Warning,
		TEXT("%s Skill %d Purchased"),
		SkillPathName,
		SkillIndex + 1
	);

	ClickedButton->InternalButtonClickedWithSource.RemoveDynamic(
		this,
		&UMainInventoryWidget::HandleSkillButtonClicked
	);

	const int32 NextSkillIndex = SkillIndex + 1;

	if (ActiveSkillButtons->IsValidIndex(NextSkillIndex))
	{
		UButtonWithText* NextButton =
			(*ActiveSkillButtons)[NextSkillIndex];

		if (NextButton)
		{
			NextButton->InternalButtonClickedWithSource.AddUniqueDynamic(
				this,
				&UMainInventoryWidget::HandleSkillButtonClicked
			);
		}
	}
}
