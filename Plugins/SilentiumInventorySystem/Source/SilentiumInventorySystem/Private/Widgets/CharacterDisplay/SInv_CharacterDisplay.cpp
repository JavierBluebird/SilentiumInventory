// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/CharacterDisplay/SInv_CharacterDisplay.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "EquipmentManagement/ProxyMesh/SInv_ProxyMeshActor.h"
#include "Kismet/GameplayStatics.h"

void USInv_CharacterDisplay::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	// Get Proxy Mesh Reference.
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, ASInv_ProxyMeshActor::StaticClass(), Actors);

	if (!Actors.IsValidIndex(0)) return;

	ASInv_ProxyMeshActor* ProxyMesh = Cast<ASInv_ProxyMeshActor>(Actors[0]);
	if (!IsValid(ProxyMesh)) return;

	Mesh = ProxyMesh->GetMesh();
}

void USInv_CharacterDisplay::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	
	if (!bIsDragging) return;

	LastPosition = CurrentPosition;
	CurrentPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());

	const float HorizontalDelta = LastPosition.X - CurrentPosition.X;

	if (!Mesh.IsValid()) return;
	Mesh->AddRelativeRotation(FRotator(0, HorizontalDelta, 0));
}

FReply USInv_CharacterDisplay::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	CurrentPosition = UWidgetLayoutLibrary::GetMousePositionOnViewport(GetOwningPlayer());
	LastPosition = CurrentPosition;
	
	bIsDragging = true;
	return FReply::Handled();
}

FReply USInv_CharacterDisplay::NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	bIsDragging = false;
	return FReply::Handled();
}

void USInv_CharacterDisplay::NativeOnMouseLeave(const FPointerEvent& InMouseEvent)
{
	Super::NativeOnMouseLeave(InMouseEvent);
	bIsDragging = false;
}


