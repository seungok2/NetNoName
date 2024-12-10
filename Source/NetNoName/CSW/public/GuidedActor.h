// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GuidedActor.generated.h"

class USphereComponent;
class APlayer_Revenant;
class AEnemy;

UCLASS()
class NETNONAME_API AGuidedActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGuidedActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Setting")
	float size = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setting")
	float destroyTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setting")
	bool isClosest = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setting")
	float speed = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "setting")
	float turnSpeed = 10.0f;


private:
	USphereComponent* sphereCol;
	APlayer_Revenant* player;
	AEnemy* enemy;

	void DestroyActor();


};
