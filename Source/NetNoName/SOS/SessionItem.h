// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SessionItem.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API USessionItem : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	class UButton* btn_Join;

	UFUNCTION()
	void Join();
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_SessionName;

	void SetInfo(FString info);
	
	
	
};
