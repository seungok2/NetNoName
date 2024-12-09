// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_RandomLighting.h"
#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "ParticleActor.h"
//#include "Net\UnrealNetwork.h"

void UNotifyState_RandomLighting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	auto me = MeshComp->GetOwner();

	if (!me)
		return;

	FVector CenterPos = me->GetActorLocation();

	UWorld* world = me->GetWorld();



	if (me->HasAuthority())
	{
		spawnPos.Empty();
		
		if (world)
		{
			for (int32 i = 0; i < spawnNum; i++)
			{
				FVector RandomDir = UKismetMathLibrary::RandomUnitVector();
				float RandomDis = FMath::FRandRange(spawnMinRadius, spawnMaxRadius);

				// 스폰 위치 계산 = 현재 위치 + 방향*거리
				spawnPos.Add(CenterPos + RandomDir * RandomDis);

				FHitResult hitInfo;
				FVector start = spawnPos[i] + FVector(0, 0, 5000.0f);
				FVector end = spawnPos[i] + FVector(0, 0, -5000.0f);

				bool bhit = world->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility);

				if (bhit)
				{
					spawnPos[i].Z = hitInfo.Location.Z + 0.1f;
				}
				else
				{
					spawnPos[i].Z = CenterPos.Z - 260.0f;
				}

				world->SpawnActor<AParticleActor>(magicCircleActor, spawnPos[i], FRotator::ZeroRotator);

			}

			AEnemy* enemy = Cast<AEnemy>(me);
			if (enemy)
			{
				enemy->Client_RandomLighting(spawnPos, magicCircleActor);
			}
		}
	}
}

void UNotifyState_RandomLighting::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	auto me = MeshComp->GetOwner();

	if (!me)
		return;


	FVector CenterPos = me->GetActorLocation();

	UWorld* world = me->GetWorld();

	// 마무리 Effect 
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = me;

	world->SpawnActor<AParticleActor>(drumParticle, CenterPos, FRotator::ZeroRotator, SpawnParams);


	if (me->HasAuthority())
	{
		if (world)
		{
			int32 Randomindex = FMath::RandRange(0, particleActors.Num() - 1);


			for (int32 i = 0; i < spawnNum; i++)
			{
				
				// Actor로 대체
				world->SpawnActor<AParticleActor>(particleActors[Randomindex], spawnPos[i], FRotator::ZeroRotator);
			}

			AEnemy* enemy = Cast<AEnemy>(me);
			if (enemy)
			{
				enemy->Client_RandomLighting(spawnPos, particleActors[Randomindex]);
			}
		}
	}

}








