// Copyright Aluminati Studios Publishing 2026. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SkillTreeComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CLOCKWORKHORRORS_API USkillTreeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USkillTreeComponent();
	UFUNCTION()
	void SkillUpgraded(int32 SkillTreePath, int32 SkillIndex);
	TArray< class USkillTreeDataAsset*> SwordSkills;
	TArray< class USkillTreeDataAsset*> StaffSkills;
	TArray< class USkillTreeDataAsset*> BlasterSkills;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill1;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill2;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill3;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill4;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill5;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill6;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill7;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill8;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill9;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* SwordSkill10;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill1;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill2;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill3;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill4;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill5;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill6;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill7;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill8;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill9;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* StaffSkill10;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill1;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill2;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill3;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill4;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill5;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill6;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill7;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill8;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill9;
	UPROPERTY(BlueprintReadWrite, EditDefaultsOnly)
	class USkillTreeDataAsset* BlasterSkill10;
private:

		
};
