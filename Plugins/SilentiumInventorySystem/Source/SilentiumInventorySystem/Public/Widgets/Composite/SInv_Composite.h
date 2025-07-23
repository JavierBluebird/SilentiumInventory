// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "SInv_CompositeBase.h"
#include "SInv_Composite.generated.h"

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_Composite : public USInv_CompositeBase
{
	GENERATED_BODY()

public:
	void NativeOnInitialized() override;

	/*----------------------------------------------*/
	/*		Composite Base Function Override		*/
	/*----------------------------------------------*/
	virtual void ApplyFunction(FuncType Function) override;
	virtual void Collapse() override;
	TArray<USInv_CompositeBase*> GetChildren() { return Children; }
	
private:
	
	UPROPERTY()
	TArray<TObjectPtr<USInv_CompositeBase>> Children;
};
