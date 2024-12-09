// Fill out your copyright notice in the Description page of Project Settings.


#include "NetGameInstance.h"
#include "OnlineSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Online/OnlineSessionNames.h"

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
		// 세션 파괴 성공 시 호출되는 함수 등록
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UNetGameInstance::OnDestroySessionComplete);
		// 세션 검색 성공 시 호출되는 함수 등록
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UNetGameInstance::OnFindSessionComplete);

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

void UNetGameInstance::DestoryMySession(FString sessionName)
{
	SessionInterface->DestroySession(FName(sessionName));
	
	
}

void UNetGameInstance::OnDestroySessionComplete(FName sessionName, bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		UE_LOG(LogTemp,Warning,TEXT("OnDestroySessionComplete : %s"), *sessionName.ToString());
	}
	else
	{
		UE_LOG(LogTemp,Warning,TEXT("OnDestoryCreateSessionFail : %s"), *sessionName.ToString());
	}
	
}

void UNetGameInstance::FindOtherSession()
{
	UE_LOG(LogTemp, Warning, TEXT("Session Search Start"));
	
	// 세션 검색 설정
	sessionSearch = MakeShared<FOnlineSessionSearch>();

	// 랜을 사용할지 사용 여부
	FName subsysName = IOnlineSubsystem::Get()->GetSubsystemName();
	UE_LOG(LogTemp, Warning, TEXT("SubSystem Name : %s"), *subsysName.ToString());
	sessionSearch->bIsLanQuery = subsysName.IsEqual(FName(TEXT("NULL")));

	// 활성화 되어있는 세션만 검색하자
	sessionSearch->QuerySettings.Set(SEARCH_PRESENCE,true,EOnlineComparisonOp::Equals);
	//sessionSearch->QuerySettings.Set(TEXT("DP_NAME"), FString(TEXT("SeSAC")), EOnlineComparisonOp::Equals);
	
	//세션을 몇개까지 검색할지
	sessionSearch->MaxSearchResults = 100;

	// 세션 검색하자
	SessionInterface->FindSessions(0,sessionSearch.ToSharedRef());
	
}

void UNetGameInstance::OnFindSessionComplete(bool bWasSuccessful)
{
	if(bWasSuccessful)
	{
		auto results = sessionSearch->SearchResults;

		for(int i = 0; i < results.Num(); i++)
		{
			FOnlineSessionSearchResult sr = results[i];

			// DP_NAME 커스텀 정보 가져오기
			FString displayName;
			sr.Session.SessionSettings.Get(TEXT("DP_NAME"), displayName);

			// 세션 만들 사람 이름 (LAN은 PC이름, Steam은 ID가 들어감)
			FString sessionCreator = sr.Session.OwningUserName;
			UE_LOG(LogTemp,Warning,TEXT("Session : %s, Creator : %s"), *displayName, *sessionCreator);

			// 세션정보를 넘겨서 SessionItem을 추가하게 하자
			FString sessionInfo = FString::Printf(TEXT("%s -  %s"), *displayName, *sessionCreator);
			onAddSession.ExecuteIfBound(sessionInfo);
			
		}
		
		
	}
	
	
	UE_LOG(LogTemp, Warning, TEXT("Session Search Complete"));
	
}
