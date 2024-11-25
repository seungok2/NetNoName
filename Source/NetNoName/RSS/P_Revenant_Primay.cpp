// Fill out your copyright notice in the Description page of Project Settings.


#include "P_Revenant_Primay.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

void AP_Revenant_Primay::BeginPlay()
{
	Super::BeginPlay();
}

void AP_Revenant_Primay::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (GetLifeSpan() <= 0)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_NoHit, GetActorTransform());
}

AP_Revenant_Primay::AP_Revenant_Primay()
{
	SetLifeSpan(2);
}

void AP_Revenant_Primay::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::OnComponentHit(HitComponent, OtherActor, OtherComp, NormalImpulse, Hit);

	if (OtherActor == nullptr || OtherComp == nullptr) return;
	
	UGameplayStatics::ApplyDamage(OtherActor, 5.0f, GetInstigatorController(), this, UDamageType::StaticClass());
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Particle_Hit_World, Hit.ImpactPoint, UKismetMathLibrary::MakeRotFromX(Hit.ImpactNormal));

	Destroy();
}
