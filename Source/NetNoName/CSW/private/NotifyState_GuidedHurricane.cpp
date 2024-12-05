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

	// 등장 Effect 이걸 구지?? 서버로??
	FActorSpawnParameters spawnParameter;
	spawnParameter.Owner = me;

	world->SpawnActor<AParticleActor>(drumParticle, CenterPos, FRotator::ZeroRotator, spawnParameter);

	if (me->HasAuthority())
	{

		if (world)
		{

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
			AEnemy* enemy = Cast<AEnemy>(me);
			if (enemy)
			{
				enemy->Client_RandomHurricaneCircle(spawnPos, spawnEffect);
			}
		}

	}

}

void UNotifyState_GuidedHurricane::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UNotifyState_GuidedHurricane::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (me->HasAuthority())
	{
		if (world)
		{
			AEnemy* enemy = Cast<AEnemy>(me);

			int32 Randomindex = FMath::RandRange(0, particleGuidedActor.Num() - 1);
			
			for (int i = 0; i < spawnNum; i++)
			{
				UE_LOG(LogTemp, Warning, TEXT("hurricane"));


				world->SpawnActor<AGuidedActor>(particleGuidedActor[Randomindex], spawnPos[i], FRotator::ZeroRotator);
				
			}

			if (enemy)
			{
				enemy->Client_RandomHurricaneActor(spawnPos, particleGuidedActor[Randomindex]);
			}
		}
	}
}
