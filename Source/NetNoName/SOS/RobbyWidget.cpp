// Fill out your copyright notice in the Description page of Project Settings.


#include "RobbyWidget.h"
#include "Components/Button.h"

void URobbyWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼이 바인딩되어 있다면 이벤트 연결
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &URobbyWidget::OnStartClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &URobbyWidget::OnQuitClicked);
	}
}

void URobbyWidget::OnStartClicked()
{
	// 게임 시작 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked!"));
}

void URobbyWidget::OnQuitClicked()
{
	// 게임 종료 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("Quit Button Clicked!"));
}