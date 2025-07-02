#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "IAutomationReport.h"
#include "Types/AttributeStorage.h"
#include "SInv_ItemFragment.generated.h"


/*----------------------------------------------------------------*/
/*															 	  */
/*					Item Fragments Parent Struct				  */
/*															 	  */
/*----------------------------------------------------------------*/

USTRUCT(BlueprintType)
struct FSInv_ItemFragment 
{
	GENERATED_BODY()

	/*----------------------------------------------------------------*/
	/*							Rule of Five					 	  */
	/*	Since we are making use of a self-declared virtual destructor */
	/*	we must declare the following five operations.				  */
	/*----------------------------------------------------------------*/

	// Constructor por defecto: permite crear el objeto sin parámetros.
	FSInv_ItemFragment() {}

	// Constructor por copia: crea un nuevo objeto copiando los valores de otro.
	FSInv_ItemFragment(const FSInv_ItemFragment&) = default;

	// Operador de asignación por copia: copia los valores de otro objeto ya existente.
	FSInv_ItemFragment& operator=(const FSInv_ItemFragment&) = default;

	// Constructor por movimiento: permite mover los datos desde un objeto temporal (rvalue).
	FSInv_ItemFragment(FSInv_ItemFragment&&) = default;

	// Operador de asignación por movimiento: permite mover datos a un objeto ya existente.
	FSInv_ItemFragment& operator=(FSInv_ItemFragment&&) = default;

	// Destructor virtual: necesario si se va a heredar de esta clase (permite destrucción correcta).
	virtual ~FSInv_ItemFragment() {}

	/*----------------------------------------------------------------*/
	/*						End of Rule of Five					 	  */
	/*----------------------------------------------------------------*/
	
	/*----------------------------------------------------------------*/
	/*				Getter and Setters for FragmentType			 	  */
	/*----------------------------------------------------------------*/
	FGameplayTag GetFragmentTag() const {return FragmentTag;}
	void SetFragmentTag(FGameplayTag Tag) {FragmentTag = Tag;}
	
	private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FGameplayTag FragmentTag = FGameplayTag::EmptyTag;
};

/*----------------------------------------------------------------*/
/*															 	  */
/*					Item Fragments Children Structs				  */
/*															 	  */
/*----------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/*						Grid Fragment						 	  */
/*----------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_GridFragment : public FSInv_ItemFragment
{
	GENERATED_BODY()

	// Getter and Setter for GridSize and Padding
	FIntPoint GetGridSize() const { return GridSize; }
	void SetGridSize(const FIntPoint& Size) { GridSize = Size; }

	float GetGridPadding() const { return GridPadding; }
	void SetGridPadding(float Padding) { GridPadding = Padding; }
	
	private:
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FIntPoint GridSize {1,1};
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float GridPadding {0.f};
};

/*------------------------------------------------*/
/*					Image Fragment			 	  */
/*------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_ImageFragment : public FSInv_ItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetItemIcon() const { return ItemIcon; }
	void SetItemIcon(UTexture2D* Icon) { ItemIcon = Icon; }

	FVector2D GetIconDimension() const { return IconDimensions; }
	void SetIconDimensions(const FVector2D& Dimensions) { IconDimensions = Dimensions; }
	
	private:
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TObjectPtr<UTexture2D> ItemIcon { nullptr };

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FVector2D IconDimensions {44.f , 44.f};
};