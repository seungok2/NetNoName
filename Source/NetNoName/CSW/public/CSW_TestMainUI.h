// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CSW_TestMainUI.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API UCSW_TestMainUI : public UUserWidget
{
	GENERATED_BODY()
	

protected:
	virtual void NativeConstruct() override;


public:
	UPROPERTY(meta = (BindWidget))
	class UNamedSlot* enemyHpSlot;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UEnemyHpBarUI> enemyHpBarFactory;

	class UEnemyHpBarUI* enemyHp;

public:
	void AddEnemeyHpBar();
	void UpdateCurrentHp(int32 current, int32 maxHp);

};
