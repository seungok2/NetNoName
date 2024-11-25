// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_NoName.generated.h"

class APlayer_Base;
/**
 * 
 */
UCLASS()
class NETNONAME_API AGM_NoName : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	TArray<APlayer_Base*> Players;

	void AddPlayer(APlayer_Base* Player);
	
};
