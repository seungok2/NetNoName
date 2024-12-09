// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobbyWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API URobbyWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// 생성자 또는 초기화 함수 선언 가능
	virtual void NativeConstruct() override;

	

	
};
