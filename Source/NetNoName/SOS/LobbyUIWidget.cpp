// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyUIWidget.h"
#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Components/ScrollBox.h"
#include "NetGameInstance.h"
#include "RobbyWidget.h"
#include "SessionItem.h"
#include "Kismet/KismetSystemLibrary.h"

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

	Quit_Sure_Yes->OnClicked.AddDynamic(this, &ULobbyUIWidget::QuitGame);
	Quit_Sure_No->OnClicked.AddDynamic(this, &ULobbyUIWidget::CloseSessionWidget);

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
	Btn_FindeSession_back->OnClicked.AddDynamic(this, &ULobbyUIWidget::OnSessionClicked);
	Btn_CreateSession_Back->OnClicked.AddDynamic(this, &ULobbyUIWidget::OnSessionClicked);
	
	// slider 값이 변경되면 호출되는 함수 등록
	slider_PlayerCount->OnValueChanged.AddDynamic(this, &ULobbyUIWidget::OnValueChange);

	// 세션 검색되면 호출되는 함수 등록
	gi->onAddSession.BindUObject(this, &ULobbyUIWidget::OnAddSession);
	// 세션 완전 검색 완료되면 호출되는 함수 등록
	gi->onFindComplete.BindUObject(this, &ULobbyUIWidget::OnFindComplete);
	
}

void ULobbyUIWidget::GoCreate()
{
	
	WidgetSwitcher->SetActiveWidgetIndex(2);
}

void ULobbyUIWidget::GoFind()
{
	WidgetSwitcher->SetActiveWidgetIndex(3);
	// 강제소 세션 검색 하자
	FindSession();
	
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
	GetWorld()->ServerTravel(TEXT("/Game/Polar/Maps/RobbyMap?listen"));
	
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
	// 기존것을 지우고 실행
	Scroll_SessionList->ClearChildren();
	
	gi->FindOtherSession();
	
}

void ULobbyUIWidget::OnAddSession(int32 idx, FString info)
{
	USessionItem* item = CreateWidget<USessionItem>(GetWorld(), sessionItemFactory);
	Scroll_SessionList->AddChild(item);
	item->SetInfo(idx, info);
	
}

void ULobbyUIWidget::OnFindComplete(bool isComplete)
{
	// 검색 버튼 비활성
	FindButton->SetIsEnabled(isComplete);
}


void ULobbyUIWidget::QuitGame()
{
	
	// 플레이어 컨트롤러 가져오기
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	// 게임 종료
	UKismetSystemLibrary::QuitGame(GetWorld(), PlayerController, EQuitPreference::Quit, true);
	
}

void ULobbyUIWidget::OnStartClicked()
{
	// 게임 시작 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("Start Button Clicked!"));
	// 세션 만든 사람(서버)이 만들어진 세션으로 이동
	GetWorld()->ServerTravel(TEXT("/Game/ModularSciFi/Levels/LandscapePreview?listen"));
}

void ULobbyUIWidget::OnQuitClicked()
{
	// 게임 종료 로직 구현
	UE_LOG(LogTemp, Warning, TEXT("Quit Button Clicked!"));

	WidgetSwitcher->SetActiveWidgetIndex(4);
	
	
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
