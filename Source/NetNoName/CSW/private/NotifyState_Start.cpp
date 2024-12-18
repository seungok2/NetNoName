// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Start.h"
#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "ParticleActor.h"

#include "Enemy.h"
#include "EnemyHpBarUI.h"

void UNotifyState_Start::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

    auto my = MeshComp->GetOwner();

	AEnemy* enemy = Cast<AEnemy>(my);

	if (my->HasAuthority())
	{
		FVector pos = my->GetActorLocation();
		FRotator rot = FRotator::ZeroRotator;

		FHitResult hitInfo;
		FVector start = pos + FVector(0, 0, 5000.0f);
		FVector end = pos + FVector(0, 0, -5000.0f);

		bool bhit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECC_Visibility);

		if (bhit)
		{
			pos.Z = hitInfo.Location.Z;
		}
		else
		{
			pos.Z -= 260.0f;
		}


		my->GetWorld()->SpawnActor<AParticleActor>(partcleActor, pos, rot);

		enemy->Client_SpawnEffect(pos, partcleActor);
	}
}

void UNotifyState_Start::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AActor* owner = MeshComp->GetOwner();
	me = Cast<AEnemy>(MeshComp->GetOwner());

	if (owner->HasAuthority())
	{
		if (me)
		{
			// 상태 Move로 변경
			me->mState = EEnemyState::Move;
		}
    }

}
