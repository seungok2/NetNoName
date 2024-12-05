// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHpBarUI.h"
#include "Components\ProgressBar.h"
#include "Components\TextBlock.h"

void UEnemyHpBarUI::NativeConstruct()
{
	//enemy hp = progress  대칭
	// Text도 대칭
	
}

void UEnemyHpBarUI::UpdateHp(int32 currentHp, int32 maxHp)
{
    if (progressBar_HpBar)
    {
        // 체력 비율 계산
        float hpRatio = FMath::Clamp(static_cast<float>(currentHp) / static_cast<float>(maxHp), 0.0f, 1.0f);
        progressBar_HpBar->SetPercent(hpRatio); // 프로그래스바 업데이트
    }

    if (txt_EnemyHp)
    {
        // 체력 텍스트 업데이트
        txt_EnemyHp->SetText(FText::FromString(FString::Printf(TEXT("%d / %d"), currentHp, maxHp)));
    }


}
