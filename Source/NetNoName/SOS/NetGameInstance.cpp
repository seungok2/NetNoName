// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"

void UNetGameInstance::Init()
{
	Super::Init();

	// 현재 사용하는 서브시스템을 가져오자
	IOnlineSubsystem* subsys = IOnlineSubsystem::Get();
	if(subsys)
	{
		SessionInterface = subsys->GetSessionInterface();
		// 세션 생성 성공 시 호출되는 함수 등록
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnCreateSessionComplete);
	}
}

void UNetGameInstance::CreateMySession(FString displayName, int32 playerCount)
{
	// 새션을 만들기 위한 세팅
	FOnlineSessionSettings SessionSettings;

	// Lan을 이용할지 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	SessionSettings.bIsLANMatch = subsysName.IsEqual(FName(TEXT("NULL")));

	// Lobby를 사용할지 여부
	SessionSettings.bUseLobbiesIfAvailable = true;

	// 세션 검색을 허용할지 여부
	SessionSettings.bShouldAdvertise = true;

	// 세션이 진행 중일때 참여 가능 여부
	SessionSettings.bAllowJoinInProgress = true;

	// 세션의 최대 인원 설정
	SessionSettings.NumPublicConnections = playerCount;

	// 커스텀 정보 key값으로 세팅 가능
	SessionSettings.Set(TEXT("DP_NAME"),displayName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// 새션 생성
	FUniqueNetIdPtr netID = GetWorld()->GetFirstLocalPlayerFromController()->GetUniqueNetIdForPlatformUser().GetUniqueNetId();
	SessionInterface->CreateSession(*netID,FName(displayName), SessionSettings);
	
}

void UNetGameInstance::OnCreateSessionComplete(FName sessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp,Warning,TEXT("OnCreateSessionComplete : %s"), *sessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("OnCreateSessionFail : %s"), *sessionName.ToString());
	}
	
}
