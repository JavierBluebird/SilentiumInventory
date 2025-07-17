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

	FGameplayTag GetGameplayTag() const {return FragmentTag ;}
	void SetGameplayTag(const FGameplayTag Tag) { FragmentTag = Tag; }

	virtual void Collapse();
	void Expand();

	using FuncType = TFunction<void(USInv_CompositeBase*)>;
	
	virtual void ApplyFunction(FuncType Function) {}
	
private:
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FGameplayTag FragmentTag;
};
