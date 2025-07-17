// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Widgets/Composite/SInv_Composite.h"
#include "SInv_ItemDescription.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_ItemDescription : public USInv_Composite
{
	GENERATED_BODY()
	
public:

	FVector2D GetBoxSize() const;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
