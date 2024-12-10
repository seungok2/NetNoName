// Fill out your copyright notice in the Description page of Project Settings.


#include "TransferToLevelUI.h"
#include "Components/Button.h"

void UTransferToLevelUI::NativeConstruct()
{
	Super::NativeConstruct();


	btn_Transfer->OnClicked.AddDynamic(this, &UTransferToLevelUI::OnClick_Transfer);


}

void UTransferToLevelUI::OnClick_Transfer()
{
	GetWorld()->ServerTravel(TEXT("/Game/Polar/Maps/RobbyMap?listen"));
}
