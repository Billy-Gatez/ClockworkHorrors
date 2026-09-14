// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonWithText.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FButtonDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
	FButtonWithSourceDelegate,
	UButtonWithText*,
	ClickedButton
);
UCLASS()
class CLOCKWORKHORRORS_API UButtonWithText : public UUserWidget
{
	GENERATED_BODY()
	
	virtual void NativePreConstruct() override;

	virtual void NativeConstruct() override;

public:

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Variables")
	FButtonDelegate InternalButtonClicked;
	UPROPERTY(BlueprintAssignable, Category = "Variables")
	FButtonWithSourceDelegate InternalButtonClickedWithSource;
protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* Information;

	UPROPERTY(BlueprintReadOnly, meta = (BindWidget))
	class UButton* BackgroundButton;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variables")
	FText Info;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variables")
	 class UImage* Image;
	 UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Variables")
	 UTexture2D* texture;
private:
	UFUNCTION()
	void HandleButtonClicked();

	UPROPERTY(EditAnywhere, Category = "Variables")
	FLinearColor TextColor = FLinearColor::White;

	UPROPERTY(EditAnywhere, Category = "Variables")
	FLinearColor ButtonColor = FLinearColor::Black;
};
