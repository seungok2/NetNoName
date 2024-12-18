// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LobbyUIWidget.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API ULobbyUIWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	// 버튼을 위한 UPROPERTY
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* QuitButton;
	// 종료버튼
	UPROPERTY(meta = (BindWidget))
	class UButton* Quit_Sure_Yes;
	UPROPERTY(meta = (BindWidget))
	class UButton* Quit_Sure_No;
	UFUNCTION()
	void QuitGame();

	UPROPERTY(meta = (BindWidget))
	class UButton* SessionButton;
	
	// 버튼 클릭 이벤트 핸들러 함수
	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnQuitClicked();
	

	UFUNCTION()
	void OnSessionClicked();

	UFUNCTION()
	void UIvisibility();
	
public:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ULobbyUIWidget> lobbyUIFactory;

	UPROPERTY()
	class ULobbyUIWidget* lobbyUI;
	
public:
	// 새션 이동 관련
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* WidgetSwitcher;
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Go_Create;
	UFUNCTION()
	void GoCreate();

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Go_Find;
	UFUNCTION()
	void GoFind();
	
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_CreateSession_Back;
	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_FindeSession_back;
	

	UPROPERTY(meta = (BindWidget))
	class UButton* Btn_Close;
	UFUNCTION()
	void CloseSessionWidget();
	
	UPROPERTY()
	class UNetGameInstance* gi;

	// 세션 생성 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* CreateButton;

	
	UFUNCTION()
	void CreateSession();

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* Edit_DisplayName;

	UPROPERTY(meta = (BindWidget))
	class USlider* slider_PlayerCount;

	UFUNCTION()
	void OnValueChange(float value);

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_PlayerCount;

	// 세션 검색 관련
	UPROPERTY(meta = (BindWidget))
	class UButton* FindButton;
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* Scroll_SessionList;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class USessionItem> sessionItemFactory;
	
	UFUNCTION()
	void FindSession();

	UFUNCTION()
	void OnAddSession(int32 idx, FString info);

	UFUNCTION()
	void OnFindComplete(bool isComplete);

	
	
};
