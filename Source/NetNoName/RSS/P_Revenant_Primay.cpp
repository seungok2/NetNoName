// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Revenant_Primay.h"

#include "Kismet/GameplayStatics.h"

void AP_Revenant_Primay::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_NoHit, GetActorLocation(), GetActorRotation());
}

AP_Revenant_Primay::AP_Revenant_Primay()
{
	SetLifeSpan(1);
}
