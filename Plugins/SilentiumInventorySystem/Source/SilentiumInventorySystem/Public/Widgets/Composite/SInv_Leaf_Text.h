// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "SInv_Leaf.h"
#include "SInv_Leaf_Text.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_Leaf_Text : public USInv_Leaf
{
	GENERATED_BODY()

public:
	void SetText(const FText& Text) const;
	
	virtual void NativePreConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_LeafText;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 FontSize {12};
};
