// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ParticleActor.generated.h"

class USphereComponent;

UCLASS()
class NETNONAME_API AParticleActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AParticleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


private:
	USphereComponent* sphereCol;

public:

	UPROPERTY(EditAnywhere, Category = "Setting")
	float size = 100.0f;

	UPROPERTY(EditAnywhere, Category = "Setting")
	float DestroyTime = 3.0f;

protected:
	void DestroyActor();
	
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

};
