// Fill out your copyright notice in the Description page of Project Settings.


#include "NotifyState_Start.h"
#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "ParticleActor.h"

#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"

#include "EnemyHpBarUI.h"

void UNotifyState_Start::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{

    auto my = MeshComp->GetOwner();

	// UI 체력바 보여주고, MainUI에 붙여준다
	//UEnemyHpBarUI* hpUI = CreateWidget<UEnemyHpBarUI>(my->GetWorld());
	

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
