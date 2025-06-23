// Fill out your copyright notice in the Description page of Project Settings.


#include "Interaction/SInv_HighlightableStaticMesh.h"

void USInv_HighlightableStaticMesh::Highlight_Implementation()
{
	SetOverlayMaterial(HighlightMaterial);
}

void USInv_HighlightableStaticMesh::UnHighlight_Implementation()
{
	SetOverlayMaterial(nullptr);
}
