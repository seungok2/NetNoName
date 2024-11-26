// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_GuidedHurricane.generated.h"

/**
 * 
 */
class UNiagaraSystem;
class AGuidedActor;
class AParticleActor;

UCLASS()
class NETNONAME_API UNotifyState_GuidedHurricane : public UAnimNotifyState
{
	GENERATED_BODY()
	

protected:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
public:
	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<UParticleSystem*> lightningEffects;*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TSubclassOf<AParticleActor> drumParticle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<TSubclassOf<AGuidedActor>> particleGuidedActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* spawnEffect;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 spawnNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float spawnMinRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float spawnMaxRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float spawnCircleDis = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float delayTime = 1.0f;

private:
	TArray<FVector> spawnPos;

	AActor* me;

	FVector CenterPos;
	UWorld* world;

};
