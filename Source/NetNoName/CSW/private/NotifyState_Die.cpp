// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Die.h"
#include "Components\SkeletalMeshComponent.h"
#include "Materials\MaterialInstanceDynamic.h"
#include "ParticleActor.h"

#include "Enemy.h"

void UNotifyState_Die::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	AActor* my = MeshComp->GetOwner();

	if (my->HasAuthority())
	{
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

		AEnemy* enemy = Cast<AEnemy>(my);
		enemy->Client_SpawnEffect(pos, p_Actor);
	}
}

void UNotifyState_Die::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
}

void UNotifyState_Die::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (!MeshComp || !MeshComp->GetWorld())
		return;

	FTimerHandle TimerHandle;

	// 5ÃÊ ÈÄ ÆÄ±«
	MeshComp->GetWorld()->GetTimerManager().SetTimer(
		TimerHandle,
		[MeshComp]()
		{
			if (AActor* Owner = MeshComp->GetOwner())
			{
				Owner->Destroy();
				UE_LOG(LogTemp, Warning, TEXT("Actor destroyed after 5 seconds."));
			}
		},
		5.0f, false);

}
