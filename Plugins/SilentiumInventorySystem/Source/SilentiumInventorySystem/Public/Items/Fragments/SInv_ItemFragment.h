#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "IAutomationReport.h"
#include "Types/AttributeStorage.h"
#include "SInv_ItemFragment.generated.h"

class USInv_CompositeBase;
class APlayerController;
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

	/*-----------------------------------------------------------------------.--*/
	/*			virtual Function for Manifesting (Initialization)		 	   */
	/*------------------------------------------------------------------------*/

	virtual void Manifest() {}
	
	private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (Categories = "FragmentTags"))
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
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Amount of Slots this Item will take on Inventory Grid"))
	FIntPoint GridSize {1,1};
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float GridPadding {0.f};
};

/*--------------------------------------------------------*/
/*														  */
/*				Inventory Item Fragment					  */
/*		Specific for Assimilation within a Widget		  */
/*														  */
/*--------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_InventoryItemFragment : public FSInv_ItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(USInv_CompositeBase* Composite) const;

protected:
	bool MatchesWidgetTag(const USInv_CompositeBase* Composite) const;
};

/*------------------------------------------------*/
/*					Image Fragment			 	  */
/*------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_ImageFragment : public FSInv_InventoryItemFragment
{
	GENERATED_BODY()

	UTexture2D* GetItemIcon() const { return ItemIcon; }
	
	virtual void Assimilate(USInv_CompositeBase* Composite) const override;
	
	void SetItemIcon(UTexture2D* Icon) { ItemIcon = Icon; }

	FVector2D GetIconDimension() const { return IconDimensions; }
	void SetIconDimensions(const FVector2D& Dimensions) { IconDimensions = Dimensions; }

	
	private:
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TObjectPtr<UTexture2D> ItemIcon { nullptr };

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FVector2D IconDimensions {44.f , 44.f};
};

/*------------------------------------------------*/
/*				Stackable Fragment			 	  */
/*------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_StackableFragment : public FSInv_ItemFragment
{
	GENERATED_BODY()
	
	int32 GetMaxStackSize() const { return MaxStackSize; }
	int32 GetStackCount() const { return StackCount; }
	void SetStackCount(int32 Count) { StackCount = Count; }
	
private:

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Max amount of items the Stack can hold per Slot."))
	int32 MaxStackSize {1};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Amount of items we will get on the Stack when Item gets picked up."))
	int32 StackCount {1};
};

/*---------------------------------------------------------*/
/*														  */
/*				Base Consumable Fragment			 	  */
/*														  */
/*--------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_ConsumableFragment : public FSInv_ItemFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) {}
};

/*---------------------------------------------------------*/
/*				Consumable Child Fragments			 	  */
/*--------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_HealthPotionFragment : public FSInv_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float HealAmount {20.f};
	
	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FSInv_ManaPotionFragment : public FSInv_ConsumableFragment
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float ManaAmount {20.f};
	
	virtual void OnConsume(APlayerController* PC) override;
};
/*------------------------------------------------*/
/*					Text Fragment			 	  */
/*------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_TextFragment : public FSInv_InventoryItemFragment
{
	GENERATED_BODY()
	
	virtual void Assimilate(USInv_CompositeBase* Composite) const override;

	FText GetText() const { return FragmentText; }
	void SetText(const FText& Text) { FragmentText = Text; }
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FText FragmentText;
};

/*------------------------------------------------*/
/*				Labeled Number Fragment			  */
/*------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_LabeledNumberFragment : public FSInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Manifest() override;
	
	virtual void Assimilate(USInv_CompositeBase* Composite) const override;

	// When Manifesting for the First Time, this fragment will randomize, however once equipped
	// and dropped, an item should retain the same value, so randomization should not occur.
	bool bRandomizeOnManifest {true};
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	FText Text_Label;

	UPROPERTY(VisibleAnywhere, Category = "Silentium Inventory")
	float Value {0.f};
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float Min {0.f};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	float Max {0.f};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	bool bCollapseLabel {false};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	bool bCollapseValue {false};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 MinFractionalDigits {1};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	int32 MaxFractionalDigits {1};
};
