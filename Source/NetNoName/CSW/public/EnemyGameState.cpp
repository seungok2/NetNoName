// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyGameState.h"
#include "EnemyHpBarUI.h"

void AEnemyGameState::BeginPlay()
{
	Super::BeginPlay();
}



template <typename T>
T* AEnemyGameState::GetGameUI(TSubclassOf<T> WidgetClass)
{
	if (!WidgetClass || !GetWorld())
		return;

	T* widget = CreateWidget<T>(GetWorld(), WidgetClass);
	if (widget)
	{
		widget->AddToViewport();
	}
	return widget;
}