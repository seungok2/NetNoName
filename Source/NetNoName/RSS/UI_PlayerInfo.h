// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI_PlayerInfo.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API UUI_PlayerInfo : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* PB_HP;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Txt_HP;

public:
	void SetHP(float currentHP, float maxHP);
};
