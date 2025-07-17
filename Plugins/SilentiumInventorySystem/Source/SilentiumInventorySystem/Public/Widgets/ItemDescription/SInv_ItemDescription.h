// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_ItemDescription.generated.h"

class USizeBox;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_ItemDescription : public UUserWidget
{
	GENERATED_BODY()
	
public:

	FVector2D GetBoxSize() const;

private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USizeBox> SizeBox;
};
