// Developed by Javier Gonzalez [Lumina Division]


#include "EquipmentManagement/ProxyMesh/SInv_ProxyMeshActor.h"

#include "EquipmentManagement/Components/SInv_EquipmentComponent.h"
#include "GameFramework/Character.h"


ASInv_ProxyMeshActor::ASInv_ProxyMeshActor()
{
	PrimaryActorTick.bCanEverTick = false;
	SetReplicates(false);
	
	RootComponent = CreateDefaultSubobject<USceneComponent>("RootComponent");
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>("Mesh");
	Mesh->SetupAttachment(RootComponent);

	EquipmentComponent = CreateDefaultSubobject<USInv_EquipmentComponent>("EquipmentComponent");
	EquipmentComponent->SetOwningSkeletalMesh(Mesh);
	EquipmentComponent->SetIsProxy(true);
}

void ASInv_ProxyMeshActor::BeginPlay()
{
	Super::BeginPlay();
	DelayedInitializeOwner();
}

void ASInv_ProxyMeshActor::DelayedInitializeOwner()
{
	UWorld* World = GetWorld();
	if (!IsValid(World)) // If BeginPlay happens too soon, waits until next Frame
	{
		DelayedInitialization();
		return;
	}
	APlayerController* PC = World->GetFirstPlayerController();

	if (!PC) // If player controller isn't valid yet, try next frame.
	{
		DelayedInitialization();
		return;
	}

	ACharacter* Character = Cast<ACharacter>(PC->GetPawn());
	
	if (!Character) // If player Character isn't valid yet, try next frame.
	{
		DelayedInitialization();
		return;
	}

	USkeletalMeshComponent* CharacterMesh = Character->GetMesh();

	if (!IsValid(CharacterMesh))
	{
		DelayedInitialization();
		return;
	}
	// Sets proxy mesh to Source Mesh, same as AnimInstance.
	SourceMesh = CharacterMesh;
	Mesh->SetSkeletalMesh(SourceMesh->GetSkeletalMeshAsset());
	Mesh->SetAnimInstanceClass(SourceMesh->GetAnimInstance()->GetClass());
	EquipmentComponent->InitializeOwner(PC);
}

void ASInv_ProxyMeshActor::DelayedInitialization()
{
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindUObject(this, &ThisClass::DelayedInitializeOwner);
	GetWorld()->GetTimerManager().SetTimerForNextTick(TimerDelegate);
}





