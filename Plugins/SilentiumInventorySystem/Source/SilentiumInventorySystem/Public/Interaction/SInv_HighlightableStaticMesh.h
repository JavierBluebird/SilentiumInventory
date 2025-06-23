// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInv_Highlightable.h"
#include "Components/StaticMeshComponent.h"
#include "SInv_HighlightableStaticMesh.generated.h"

/**
 * 
 */
UCLASS()
class SILENTIUMINVENTORYSYSTEM_API USInv_HighlightableStaticMesh : public UStaticMeshComponent, public ISInv_Highlightable
{
	GENERATED_BODY()

	public:
	
	virtual void Highlight_Implementation() override;
	virtual void UnHighlight_Implementation() override;

	private:
	UPROPERTY(EditAnywhere, Category="Silentium Inventory System")
	TObjectPtr<UMaterialInterface> HighlightMaterial;
};
