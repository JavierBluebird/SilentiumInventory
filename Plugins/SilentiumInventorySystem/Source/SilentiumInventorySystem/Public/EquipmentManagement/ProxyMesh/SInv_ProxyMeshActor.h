// Developed by Javier Gonzalez [Lumina Division]

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SInv_ProxyMeshActor.generated.h"

class USInv_EquipmentComponent;

UCLASS()
class SILENTIUMINVENTORYSYSTEM_API ASInv_ProxyMeshActor : public AActor
{
	GENERATED_BODY()

public:
	
	ASInv_ProxyMeshActor();

protected:

	virtual void BeginPlay() override;

private:
	// This is the mesh on the Player-Controller Character
	TWeakObjectPtr<USkeletalMeshComponent> SourceMesh;
	
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USInv_EquipmentComponent> EquipmentComponent;

	// This is the Proxy Mesh we will see in the Inventory Menu.
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USkeletalMeshComponent> Mesh;

	FTimerHandle TimerForNextTick;
	void DelayedInitializeOwner();
	void DelayedInitialization();
};
