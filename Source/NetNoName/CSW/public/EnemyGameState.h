// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Blueprint/UserWidget.h"
#include "EnemyGameState.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API AEnemyGameState : public AGameStateBase
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;


public:

	//class UEnemyHpBarUI* GetEnemyHpUI();

	template <typename T>
	T* GetGameUI(TSubclassOf<T> WidgetClass);

public:
	// 여기서 다 만들수가 있는데?
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UEnemyHpBarUI> enemyHpBarUIFactory;

	// 생각해보니깐 Enemy는 등장씬이 있자나?? 그외 필요한게 구지 있나??

	/*UPROPERTY()
	class UEnemyHpBarUI* enemyHpBarUI;*/

	

};


