// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_RandomLighting.generated.h"

/**
 * 
 */
class AParticleActor;
class UNiagaraSystem;
//class UNiagaraComponent;

UCLASS()
class NETNONAME_API UNotifyState_RandomLighting : public UAnimNotifyState
{
	GENERATED_BODY()
	

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	//TArray<UParticleSystem*> lightningEffects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<TSubclassOf<AParticleActor>> particleActors;
	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* magicCircle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float spawnMinRadius = 100.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	float spawnMaxRadius = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	int32 spawnNum = 5;


private:
	// 스킬 생성 위치
	TArray<FVector> spawnPos;

	// 생선된 나이아가라
	//TArray<UNiagaraComponent*> niagaraComp;

};
