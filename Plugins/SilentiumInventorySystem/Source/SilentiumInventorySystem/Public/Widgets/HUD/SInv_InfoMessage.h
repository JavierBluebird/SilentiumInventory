// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SInv_InfoMessage.generated.h"

class UTextBlock;
/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_InfoMessage : public UUserWidget
{
	GENERATED_BODY()

public:

	virtual void NativeOnInitialized() override;
	
	UFUNCTION(BlueprintImplementableEvent, Category = "SilentiumInventory")
	void MessageShow();

	UFUNCTION(BlueprintImplementableEvent, Category = "SilentiumInventory")
	void MessageHide();

	void SetMessage(const FText& Message);

private:
	
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UTextBlock> TextMessage;

	UPROPERTY(EditAnywhere, Category = "SilentiumInventory")
	float MessageLifetime = 3.0f;
	
	FTimerHandle MessageTimerHandle;
	bool bIsMessageActive{false};
};
