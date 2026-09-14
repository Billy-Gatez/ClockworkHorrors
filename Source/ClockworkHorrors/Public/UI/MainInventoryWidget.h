// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Utils/InventoryItemEntryStruct.h"
#include "MainInventoryWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillDelegate,int32,path,FString,skills);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkillTreeDelegate, int32, path, int32, index);
UCLASS()
class CLOCKWORKHORRORS_API UMainInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UMainInventoryWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UInventoryGridWidget* InventoryGrid;
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* SkillPointsText;
	UPROPERTY(meta = (BindWidget))
	class UItemActionWidget* ItemActionWidget;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuTabSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* InventoryButton;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SkillTreeButton;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordSkillTreeButton;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffSkillTreeButton;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterSkillTreeButton;
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* PathSwitcher;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade1;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade2;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade3;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade4;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade5;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade6;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade7;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade8;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade9;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* SwordUpgrade10;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade1;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade2;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade3;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade4;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade5;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade6;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade7;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade8;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade9;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* StaffUpgrade10;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade1;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade2;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade3;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade4;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade5;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade6;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade7;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade8;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade9;
	UPROPERTY(meta = (BindWidget))
	class UButtonWithText* BlasterUpgrade10;
	FSkillDelegate LoadSkillTreeDelegate;
	FSkillTreeDelegate OnUpgradeSkill;
	UFUNCTION(BlueprintCallable)
	void ShowInventory(bool bInventoryShown);

	UFUNCTION(BlueprintCallable)
	void HandleSlotPressed(const FInventorySlotEntry& ItemEntry, int32 SlotIndex, FVector2D ScreenPosition);

	UFUNCTION(BlueprintCallable)
	void HandleDragStarted();

	UFUNCTION()
	void UpdateInventoryUI(const FInventorySlotEntry& ItemEntry, int32 SlotIndex, bool ItemSwaped);

	UFUNCTION()
	void UpdateInventoryUI_Remove(FName ItemName, int32 Quantity, int32 SlotIndex);

	UFUNCTION()
	void SetInventoryGridMaxSlotCount(int32 NewSlotCount);
	UFUNCTION()
	void InventoryButtonClicked();
	UFUNCTION()
	void SkillTreeButtonClicked();
	UFUNCTION()
	void SwordSkillTreeButtonClicked();
	UFUNCTION()
	void StaffSkillTreeButtonClicked();
	UFUNCTION()
	void BlasterSkillTreeButtonClicked();
	//UFUNCTION()
	//void SwordSkillOneClicked();
	//UFUNCTION()
	//void SwordSkillTwoClicked();
	//UFUNCTION()
	//void SwordSkillThreeClicked();
	//UFUNCTION()
	//void SwordSkillFourClicked();
	//UFUNCTION()
	//void SwordSkillFiveClicked();
	//UFUNCTION()
	//void SwordSkillSixClicked();
	//UFUNCTION()
	//void SwordSkillSevenClicked();
	//UFUNCTION()
	//void SwordSkillEightClicked();
	//UFUNCTION()
	//void SwordSkillNineClicked();
	//UFUNCTION()
	//void SwordSkillTenClicked();
	//UFUNCTION()
	//void StaffSkillOneClicked();
	//UFUNCTION()
	//void StaffSkillTwoClicked();
	//UFUNCTION()
	//void StaffSkillThreeClicked();
	//UFUNCTION()
	//void StaffSkillFourClicked();
	//UFUNCTION()
	//void StaffSkillFiveClicked();
	//UFUNCTION()
	//void StaffSkillSixClicked();
	//UFUNCTION()
	//void StaffSkillSevenClicked();
	//UFUNCTION()
	//void StaffSkillEightClicked();
	//UFUNCTION()
	//void StaffSkillNineClicked();
	//UFUNCTION()
	//void StaffSkillTenClicked();
	//UFUNCTION()
	//void BlasterSkillOneClicked();
	//UFUNCTION()
	//void BlasterSkillTwoClicked();
	//UFUNCTION()
	//void BlasterSkillThreeClicked();
	//UFUNCTION()
	//void BlasterSkillFourClicked();
	//UFUNCTION()
	//void BlasterSkillFiveClicked();
	//UFUNCTION()
	//void BlasterSkillSixClicked();
	//UFUNCTION()
	//void BlasterSkillSevenClicked();
	//UFUNCTION()
	//void BlasterSkillEightClicked();
	//UFUNCTION()
	//void BlasterSkillNineClicked();
	//UFUNCTION()
	//void BlasterSkillTenClicked();
	UFUNCTION()
	void SkillPointsTextUpdated(int points);
	int32 SkillTreePath;
	FString skillSelection = "0000000000";
	bool loadingData;
	class UExperienceComponent* expComp;
	void SetExperienceComponent();
	UFUNCTION()
	void LoadSkillTree(int32 SkillPath, FString skillTreeSelection);
	UFUNCTION()
	void HandleSkillButtonClicked(UButtonWithText* ClickedButton);
	private:
	TArray<UButtonWithText*> SwordSkillButtons;
	TArray<UButtonWithText*> StaffSkillButtons;
	TArray<UButtonWithText*> BlasterSkillButtons;
};
