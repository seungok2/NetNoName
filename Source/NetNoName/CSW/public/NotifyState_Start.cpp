// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Start.h"
#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "ParticleActor.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

void UNotifyState_Start::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

    auto my = MeshComp->GetOwner();

    FVector pos = my->GetActorLocation();
    FRotator rot = FRotator::ZeroRotator;
    pos.Z -= 180.0f;

    my->GetWorld()->SpawnActor<AParticleActor>(partcleActor,pos, rot);

}

void UNotifyState_Start::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    AActor* owner = MeshComp->GetOwner();
    me = Cast<AEnemy>(MeshComp->GetOwner());

    if (me)
    {
	    // 상태 Move로 변경
        me->mState = EEnemyState::Move;
    }

}
