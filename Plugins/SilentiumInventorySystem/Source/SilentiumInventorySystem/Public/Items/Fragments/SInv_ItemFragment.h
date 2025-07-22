#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "IAutomationReport.h"
#include "StructUtils/InstancedStruct.h"
#include "Types/AttributeStorage.h"
#include "SInv_ItemFragment.generated.h"

class ASInv_EquipActor;
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

	float GetValue() const { return Value; }
	FText GetTextLabel() const { return Text_Label;}
	
private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Name of the value."))
	FText Text_Label;

	UPROPERTY(VisibleAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Actual Value."))
	float Value {0.f};
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "If Randomizing, the minimum amount to random from. If Randomizing isn't needed, make it the same value as Max"))
	float Min {0.f};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "If Randomizing, the max amount to random. If Randomizing isn't needed, make it the same value as Min"))
	float Max {0.f};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Set to true if the Label should be collapsed."))
	bool bCollapseLabel {false};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Set to true if the Value should be collapsed."))
	bool bCollapseValue {false};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Minimum amount of Fractional Digits to show, set to 0 if you don't want fractional digits."))
	int32 MinFractionalDigits {1};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Maximum amount of Fractional Digits to show, set to 0 if you don't want fractional digits."))
	int32 MaxFractionalDigits {1};
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
/*			Consumable Modifiers Sub-Fragments			  */
/*--------------------------------------------------------*/

USTRUCT(BlueprintType)
struct FSInv_ConsumeModifier : public FSInv_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnConsume(APlayerController* PC) {}
};
/*---------------------------------------------------------*/
/*														  */
/*				Base Consumable Fragment			 	  */
/*														  */
/*--------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_ConsumableFragment : public FSInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(USInv_CompositeBase* Composite) const override;
	
	virtual void OnConsume(APlayerController* PC);
	virtual void Manifest() override;

	private:
	
	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ExcludeBaseStruct), meta = (ToolTip = "Array of Consume Modifiers. Add to customize Consumable Mechanics or Features."))
	TArray<TInstancedStruct<FSInv_ConsumeModifier>> ConsumeModifiers;
};


/*---------------------------------------------------------*/
/*				Consumable Child Fragments			 	  */
/*--------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_HealthPotionFragment : public FSInv_ConsumeModifier
{
	GENERATED_BODY()
	
	virtual void OnConsume(APlayerController* PC) override;
};

USTRUCT(BlueprintType)
struct FSInv_ManaPotionFragment : public FSInv_ConsumeModifier
{
	GENERATED_BODY()
	
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


/*---------------------------------------------------------*/
/*														  */
/*					Equipment Fragments	 		 	      */
/*														  */
/*--------------------------------------------------------*/

/*-----------------------------------------------------------------*/
/*				Base Equipment Modifier Fragment			 	  */
/*----------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_EquipModifier : public FSInv_LabeledNumberFragment
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) {}
	virtual void OnUnequip(APlayerController* PC) {}
};

/*-----------------------------------------------------------------*/
/*				Equipment Stats Modifier Fragments			 	  */
/*----------------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_StrengthModifier : public FSInv_EquipModifier
{
	GENERATED_BODY()

	virtual void OnEquip(APlayerController* PC) override;
	virtual void OnUnequip(APlayerController* PC) override;
};

/*---------------------------------------------------------*/
/*				 Base Equipment Fragment			 	  */
/*--------------------------------------------------------*/
USTRUCT(BlueprintType)
struct FSInv_EquipmentFragment : public FSInv_InventoryItemFragment
{
	GENERATED_BODY()

	virtual void Assimilate(USInv_CompositeBase* Composite) const override;
	bool bEquipped {false};
	void OnEquip(APlayerController* PC);
	void OnUnequip(APlayerController* PC);
	virtual void Manifest() override;

	ASInv_EquipActor* SpawnAttachedActor(USkeletalMeshComponent* AttachMesh) const;
	void DestroyAttachedActor() const;
	FGameplayTag GetEquipmentTag() const { return EquipmentType; }
	void SetEquippedActor(ASInv_EquipActor* EquipActor);
	
private:

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Attributes modifiers must be added individually to this Array."))
	TArray<TInstancedStruct<FSInv_EquipModifier>> EquipModifiers;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory")
	TSubclassOf<ASInv_EquipActor> EquipActorClass = nullptr;
	
	TWeakObjectPtr<ASInv_EquipActor> EquippedActor = nullptr;

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Socket in the Mesh this Equipment will be attached to."))
	FName SocketAttachPoint{NAME_None};

	UPROPERTY(EditAnywhere, Category = "Silentium Inventory", meta = (ToolTip = "Tag related to the Equipment Type for System Detection."))
	FGameplayTag EquipmentType = FGameplayTag::EmptyTag;
};
