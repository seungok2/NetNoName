// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHpBarUI.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API UEnemyHpBarUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(meta=(BindWidget))
	class UImage* img_BossImg;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_EnemyName;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar* progressBar_HpBar;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* txt_EnemyHp;


};
