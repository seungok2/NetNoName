// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PlayerInfo.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UUI_PlayerInfo::SetHP(float currentHP, float maxHP)
{
	float percent = currentHP / maxHP;
	PB_HP->SetPercent(percent);
	
	FLinearColor progressbarColor = FLinearColor::Green;
	progressbarColor.R = 1.0f - percent;
	progressbarColor.G = percent;
	progressbarColor.B = 0;
	progressbarColor.A = 1;
	
	PB_HP->SetFillColorAndOpacity(progressbarColor);
	FString strHP = FString::Printf( TEXT("%.f/%.f"), currentHP, maxHP); 
	Txt_HP->SetText(FText::FromString(strHP));
}