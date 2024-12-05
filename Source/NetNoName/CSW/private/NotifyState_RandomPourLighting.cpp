// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_RandomPourLighting.h"
#include "Enemy.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet\GameplayStatics.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "ParticleActor.h"

void UNotifyState_RandomPourLighting::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	me = MeshComp->GetOwner();

	if (!me)
		return;

	CenterPos = me->GetActorLocation();

	world = me->GetWorld();

	float spawnNumFloat = static_cast<float>(spawnNum*2);

	circleIndex = 0;
	magicIndex = 0;

	// 딜레이 = 재생시간 / 1초 / 간격
	delayTime = 300.0f / 60.0f / spawnNumFloat*2;
	spawnPos.Empty();

	FActorSpawnParameters spawnParameter;
	spawnParameter.Owner = me;
	
	world->SpawnActor<AParticleActor>(drumPartcle, CenterPos, FRotator::ZeroRotator, spawnParameter);


	for (int i = 0; i < spawnNum; i++)
	{
		FVector randomDir = UKismetMathLibrary::RandomUnitVector();
		float randomDis = FMath::RandRange(spawnMinRadius, spawnMaxRadius);

		spawnPos.Add(CenterPos + randomDir * randomDis);
		
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

		//spawnPos[i].Z = 0.1f;
		// spawn 나이아가라 서클 
	}


	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, TEXT("NotifyBegin"));
}

void UNotifyState_RandomPourLighting::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	currentTime += FrameDeltaTime;

	if (currentTime >= delayTime)
	{
		currentTime = 0;
		
		if (circleIndex < spawnNum)
		{
			// circle 생성하고
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(world, magicCircle, spawnPos[circleIndex], FRotator::ZeroRotator);

			circleIndex++;
		}
		else
		{
			if (magicIndex > spawnNum)
				return;

			// 천둥 치고
			int32 Randomindex = FMath::RandRange(0, particleActors.Num() - 1);

			/*UGameplayStatics::SpawnEmitterAtLocation(world, lightningEffects[Randomindex], spawnPos[magicIndex], FRotator::ZeroRotator);*/

			world->SpawnActor<AParticleActor>(particleActors[Randomindex], spawnPos[magicIndex], FRotator::ZeroRotator);

			magicIndex++;
		}
	}


    //GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("NotifyTick"));
}

void UNotifyState_RandomPourLighting::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("NotifyEnd"));
}
