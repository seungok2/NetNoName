// Fill out your copyright notice in the Description page of Project Settings.


#include "CSW_TestMainUI.h"
#include "Components\NamedSlot.h"
#include "EnemyHpBarUI.h"

void UCSW_TestMainUI::NativeConstruct()
{
	Super::NativeConstruct();


	AddEnemeyHpBar();

}

void UCSW_TestMainUI::AddEnemeyHpBar()
{
	if (!enemyHpBarFactory) return;

	enemyHp = CreateWidget<UEnemyHpBarUI>(GetWorld(), enemyHpBarFactory);
	// 체력 갱신 함수 하나 만들기
	// slot에 붙이기
	enemyHpSlot->AddChild(enemyHp);

}

void UCSW_TestMainUI::UpdateCurrentHp(int32 current, int32 maxHp)
{
	enemyHp->UpdateHp(current, maxHp);
}
