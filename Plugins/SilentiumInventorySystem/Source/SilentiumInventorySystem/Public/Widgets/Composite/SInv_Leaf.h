// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "SInv_CompositeBase.h"
#include "SInv_Leaf.generated.h"

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_Leaf : public USInv_CompositeBase
{
	GENERATED_BODY()

public:
	virtual void ApplyFunction(FuncType Function) override;
};
