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

	// 예: 버튼을 위한 UPROPERTY
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;

	// 버튼 클릭 이벤트 핸들러 함수
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();
	
};
