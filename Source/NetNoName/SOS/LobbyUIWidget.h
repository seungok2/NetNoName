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
	class UButton* Btn_Close;
	UFUNCTION()
	void CloseSessionWidget();
	
	UPROPERTY()
	class UNetGameInstance* gi;
	
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

	UPROPERTY(meta = (BindWidget))
	class UButton* FindButton;
	UFUNCTION()
	void FindSession();
	
	
	
};
