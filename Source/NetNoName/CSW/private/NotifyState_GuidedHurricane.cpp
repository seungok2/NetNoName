// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_GuidedHurricane.h"
#include "Enemy.h"
#include "Kismet\KismetMathLibrary.h"
#include "Kismet\KismetSystemLibrary.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GuidedActor.h"
#include "ParticleActor.h"

void UNotifyState_GuidedHurricane::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	me = MeshComp->GetOwner();

	if (!me)
		return;

	CenterPos = me->GetActorLocation();

	world = me->GetWorld();
	spawnPos.Empty();

	if (world)
	{
		FActorSpawnParameters spawnParameter;
		spawnParameter.Owner = me;

		world->SpawnActor<AParticleActor>(drumParticle, CenterPos, FRotator::ZeroRotator, spawnParameter);

		for (int i = 0; i < spawnNum; i++)
		{
			FVector randVector = UKismetMathLibrary::RandomUnitVector();
			float randRange = FMath::FRandRange(spawnMinRadius, spawnMaxRadius);

			// 스폰 위치 계산 = 현재 위치 + 방향*거리
			spawnPos.Add(CenterPos + randVector * randRange);
			

			FHitResult hitInfo;
			FVector start = spawnPos[i] + FVector(0, 0, 5000.0f);
			FVector end = spawnPos[i] + FVector(0, 0, -5000.0f);

			bool bhit = world->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility);

			if (bhit)
			{
				spawnPos[i].Z = hitInfo.Location.Z;
			}
			else
			{
				spawnPos[i].Z = CenterPos.Z - 260.0f;
			}

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(world, spawnEffect, spawnPos[i], FRotator::ZeroRotator);

		}
	}


}

void UNotifyState_GuidedHurricane::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UNotifyState_GuidedHurricane::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (world)
	{
		for (int i = 0; i < spawnNum; i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("hurricane"));

			int32 Randomindex = FMath::RandRange(0, particleGuidedActor.Num() - 1);

			world->SpawnActor<AGuidedActor>(particleGuidedActor[Randomindex], spawnPos[i], FRotator::ZeroRotator);
		}
	}
}
