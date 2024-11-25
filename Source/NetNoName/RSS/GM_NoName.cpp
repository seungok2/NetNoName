// Fill out your copyright notice in the Description page of Project Settings.


#include "GM_NoName.h"
#include "Player_Base.h"

void AGM_NoName::AddPlayer(APlayer_Base* Player)
{
	Players.AddUnique(Player);
}