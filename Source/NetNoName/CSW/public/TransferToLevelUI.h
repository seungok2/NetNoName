// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TransferToLevelUI.generated.h"

/**
 * GetWorld()->ServerTravel(TEXT("/Game/ModularSciFi/Levels/LandscapePreview?listen"));
 */
UCLASS()
class NETNONAME_API UTransferToLevelUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Transfer;

	UFUNCTION()
	void OnClick_Transfer();
	
};
