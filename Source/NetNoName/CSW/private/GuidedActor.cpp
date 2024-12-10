// Fill out your copyright notice in the Description page of Project Settings.


#include "GuidedActor.h"
#include "Components\SphereComponent.h"
#include "TimerManager.h"

#include "Enemy.h"
#include "NetNoName\RSS\Player_Revenant.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet\KismetMathLibrary.h"

// Sets default values
AGuidedActor::AGuidedActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("sphereColl"));
	SetRootComponent(sphereCol);
	sphereCol->InitSphereRadius(size);

	enemy = nullptr;
	player = nullptr;

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AGuidedActor::BeginPlay()
{
	Super::BeginPlay();
	
	FTimerHandle handle;
	//GetWorld()->GetTimerManager().SetTimer(handle, this, &AGuidedActor::DestroyActor, destroyTime, false);

	GetWorld()->GetTimerManager().SetTimer(
		handle,
		[this]() 
		{
			if (IsValid(this))
			{
				Destroy();
			}
		},
		destroyTime, false);



	AActor* foundActor = UGameplayStatics::GetActorOfClass(GetWorld(), AEnemy::StaticClass());
	if (foundActor)
	{
		enemy = Cast<AEnemy>(foundActor);
		if (enemy)
		{
			if (isClosest)
			{
				player = enemy->FindClosestPlayer();

			}
			else
			{
				player = enemy->FindFarthestPlayer();
			}
		}
	}
}

// Called every frame
void AGuidedActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (player)
	{
		FVector dir = (player->GetActorLocation() - GetActorLocation());
		dir.Normalize();

		FVector curDir = GetActorForwardVector();

		// 반향 전환
		FVector newDir = FMath::VInterpTo(curDir, dir, DeltaTime, turnSpeed);
		
		// 이동
		FVector velocity = newDir * speed * DeltaTime;
		SetActorLocation(GetActorLocation() + velocity);

		// 회전 설정
		FRotator NewRotation = UKismetMathLibrary::MakeRotFromX(newDir);
		SetActorRotation(NewRotation);
	}

}

void AGuidedActor::DestroyActor()
{
	if (IsValid(this))
	{
		Destroy();
	}
}



