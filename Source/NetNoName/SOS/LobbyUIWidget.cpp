// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUIWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "NetGameInstance.h"
#include "RobbyWidget.h"

void ULobbyUIWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// 버튼이 바인딩되어 있다면 이벤트 연결
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &ULobbyUIWidget::OnStartClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked.AddDynamic(this, &ULobbyUIWidget::OnQuitClicked);
	}

	if (SessionButton)
	{
		SessionButton->OnClicked.AddDynamic(this, &ULobbyUIWidget::OnSessionClicked);
	}
	
	// 게임 인스턴스 찾아오자
	gi = Cast<UNetGameInstance>( GetGameInstance() );
	
	
	CreateButton->OnClicked.AddDynamic(this, &ULobbyUIWidget::CreateSession);
	Btn_Go_Create->OnClicked.AddDynamic(this, &ULobbyUIWidget::GoCreate);
	Btn_Go_Find->OnClicked.AddDynamic(this, &ULobbyUIWidget::GoFind);
	Btn_Close->OnClicked.AddDynamic(this, &ULobbyUIWidget::CloseSessionWidget);
	FindButton->OnClicked.AddDynamic(this, &ULobbyUIWidget::FindSession);
	
	
	// slider 값이 변경되면 호출되는 함수 등록
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyUIWidget::OnValueChange);
}

void ULobbyUIWidget::GoCreate()
{
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

void ULobbyUIWidget::GoFind()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
	
}

void ULobbyUIWidget::CloseSessionWidget()
{
	WidgetSwitcher->SetActiveWidgetIndex(0);
	//RemoveFromParent();
}

void ULobbyUIWidget::CreateSession()
{
	// edit_DeisplayName 에 값이 없으면 함수 나가자.
	if(Edit_DisplayName->GetText().IsEmpty()) return;
	
	// 세션 생성
	FString displayName = Edit_DisplayName->GetText().ToString();
	int32 playerCount = slider_PlayerCount->GetValue();
	gi->CreateMySession(displayName,playerCount);
	
}

void ULobbyUIWidget::OnValueChange(float value)
{
	// 최대 인원 설정
	Text_PlayerCount->SetText(FText::AsNumber(value));
	
}

void ULobbyUIWidget::FindSession()
{
	// 세션 찾기 로직 함수 실행
	UE_LOG(LogTemp, Warning, TEXT("Find Button Clicked!"));
	
	gi->FindOtherSession();
	
}


void ULobbyUIWidget::OnStartClicked()
{
	// 게임 시작 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked!"));
}

void ULobbyUIWidget::OnQuitClicked()
{
	// 게임 종료 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("Quit Button Clicked!"));
}

void ULobbyUIWidget::OnSessionClicked()
{
	// Lobby UI 화면에 붙히자.
	/*
	lobbyUI = CreateWidget<ULobbyUIWidget>(GetWorld(), lobbyUIFactory);
	lobbyUI->AddToViewport();
	*/
	//마우스 활성화 하자.
	APlayerController* pc = GetWorld()->GetFirstPlayerController();
	pc->SetShowMouseCursor(true);
	WidgetSwitcher->SetActiveWidgetIndex(1);
	// SetVisibility(ESlateVisibility::Collapsed);
	
	// Input Mode를 UI만 동작하게 하자
}

void ULobbyUIWidget::UIvisibility()
{
	SetVisibility(ESlateVisibility::Collapsed);
}
