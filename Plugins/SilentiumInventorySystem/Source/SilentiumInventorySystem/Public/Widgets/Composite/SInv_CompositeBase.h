// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Blueprint/UserWidget.h"
#include "SInv_CompositeBase.generated.h"

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_CompositeBase : public UUserWidget
{
	GENERATED_BODY()

public:

	FGameplayTag GetFragmentTag() const {return FragmentTag ;}
	void SetGameplayTag(const FGameplayTag Tag) { FragmentTag = Tag; }

	virtual void Collapse();
	void Expand();

	using FuncType = TFunction<void(USInv_CompositeBase*)>;
	
	virtual void ApplyFunction(FuncType Function) {}
	
private:
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (Categories = "FragmentTags", ToolTip = "This tag must match Item's Fragment Tag in order to show the property on the Widget."))
	FGameplayTag FragmentTag;
};
