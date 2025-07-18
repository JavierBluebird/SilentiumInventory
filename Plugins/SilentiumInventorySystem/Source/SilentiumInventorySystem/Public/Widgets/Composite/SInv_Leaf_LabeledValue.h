// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "SInv_Leaf.h"
#include "SInv_Leaf_LabeledValue.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_Leaf_LabeledValue : public USInv_Leaf
{
	GENERATED_BODY()
public:

	void SetText_Label(const FText& Text, const bool bCollapse) const;
	void SetText_Value(const FText& Text, const bool bCollapse) const;
	virtual void NativePreConstruct() override;
	
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Label;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Value;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 FontSize_Label {12};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 FontSize_Value {18};
};
