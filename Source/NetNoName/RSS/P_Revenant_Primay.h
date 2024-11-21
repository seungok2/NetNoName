// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile_Particle_Cascade.h"
#include "P_Revenant_Primay.generated.h"

/**
 * 
 */
UCLASS()
class NETNONAME_API AP_Revenant_Primay : public AProjectile_Particle_Cascade
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* Particle_Hit_Character;

	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* Particle_Hit_World;

	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* Particle_Hit_Water;

	UPROPERTY(EditAnywhere, Category = Particle)
	UParticleSystem* Particle_NoHit;

protected:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	AP_Revenant_Primay();
};
