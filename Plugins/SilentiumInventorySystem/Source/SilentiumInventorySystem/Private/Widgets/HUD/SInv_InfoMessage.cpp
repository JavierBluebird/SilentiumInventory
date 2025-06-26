// Developed by Javier Gonzalez [Lumina Division]


#include "Widgets/HUD/SInv_InfoMessage.h"

#include "Components/TextBlock.h"

void USInv_InfoMessage::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	TextMessage->SetText(FText());
	MessageHide();
}

void USInv_InfoMessage::SetMessage(const FText& Message)
{
	TextMessage->SetText(Message);

	if (!bIsMessageActive)
	{
		MessageShow();
	}
	bIsMessageActive = true;

	GetWorld()->GetTimerManager().SetTimer(MessageTimerHandle,[this]() // lambda
	{
		MessageHide();
		bIsMessageActive = false;
	},MessageLifetime,false);
}
