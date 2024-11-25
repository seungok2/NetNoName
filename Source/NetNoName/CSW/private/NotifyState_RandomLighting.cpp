// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_RandomLighting.h"
#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
//#include "NiagaraComponent.h"
#include "ParticleActor.h"


void UNotifyState_RandomLighting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	auto me = MeshComp->GetOwner();

	if (!me)
		return;

	FVector CenterPos = me->GetActorLocation();

	UWorld* world = me->GetWorld();

	spawnPos.Empty();

	if (world)
	{
		for (int32 i = 0; i < spawnNum; i++)
		{
			FVector RandomDir = UKismetMathLibrary::RandomUnitVector();
			float RandomDis = FMath::FRandRange(spawnMinRadius, spawnMaxRadius);

			// 스폰 위치 계산 = 현재 위치 + 방향*거리
			spawnPos.Add(CenterPos + RandomDir * RandomDis);
			spawnPos[i].Z = 0.1f;

			UNiagaraFunctionLibrary::SpawnSystemAtLocation(world, magicCircle, spawnPos[i], FRotator::ZeroRotator);

			//// 디버그 점 표시 (색상, 크기, 지속 시간 조절 가능)
			//DrawDebugPoint(world, spawnPos[i], 10.0f, FColor::Blue, false, 2.0f);

			//// 또는 디버그 구 표시
			//DrawDebugSphere(world, spawnPos[i], 50.0f, 12, FColor::Green, false, 2.0f);
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

	if (world)
	{
		for (int32 i = 0; i < spawnNum; i++)
		{
			
			//niagaraComp[i]->DestroyComponent();

			int32 Randomindex = FMath::RandRange(0, particleActors.Num() - 1);

			/*UGameplayStatics::SpawnEmitterAtLocation(world, lightningEffects[Randomindex], spawnPos[i], FRotator::ZeroRotator);*/
		
			// Actor로 대체
			world->SpawnActor<AParticleActor>(particleActors[Randomindex], spawnPos[i], FRotator::ZeroRotator);

			//// 디버그 점 표시 (색상, 크기, 지속 시간 조절 가능)
			//DrawDebugPoint(world, spawnPos[i], 10.0f, FColor::Blue, false, 2.0f);

			//// 또는 디버그 구 표시
			//DrawDebugSphere(world, spawnPos[i], 50.0f, 12, FColor::Green, false, 2.0f);
		}
	}

}
