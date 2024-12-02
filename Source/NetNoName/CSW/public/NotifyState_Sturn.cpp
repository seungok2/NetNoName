// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Sturn.h"
#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "ParticleActor.h"

void UNotifyState_Sturn::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (p_Actor)
	{
		AActor* my = MeshComp->GetOwner();

		FVector pos = my->GetActorLocation();
		FRotator rot = FRotator::ZeroRotator;

		FHitResult hitInfo;
		FVector start = pos + FVector(0, 0, 5000.0f);
		FVector end = pos + FVector(0, 0, -5000.0f);

		bool bHit = GetWorld()->LineTraceSingleByChannel(hitInfo, start, end, ECollisionChannel::ECC_Visibility);

		if (bHit)
		{
			// ¶¥ÀÌ ¸ÂÀ¸¸é
			pos.Z = hitInfo.Location.Z;
		}
		else
		{
			pos.Z = -250.0f;
		}


		my->GetWorld()->SpawnActor<AParticleActor>(p_Actor, pos, rot);
	}
}

void UNotifyState_Sturn::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	me = Cast<AEnemy>(MeshComp->GetOwner());

	if (me)
	{
		me->mState = EEnemyState::Move;
	}
}
