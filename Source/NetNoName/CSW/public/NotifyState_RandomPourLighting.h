// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "NotifyState_RandomPourLighting.generated.h"

/**
 * 
 */
class UNiagaraSystem;
class AParticleActor;
UCLASS()
class NETNONAME_API UNotifyState_RandomPourLighting : public UAnimNotifyState
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
	TSubclassOf<AParticleActor> drumPartcle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	TArray<TSubclassOf<AParticleActor>> particleActors;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	UNiagaraSystem* magicCircle;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 spawnNum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 spawnMinRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 spawnMaxRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float delayTime = 1.0f;

private:
	TArray<FVector> spawnPos;

	int32 circleIndex = 0;
	int32 magicIndex = 0;

	float currentTime = 0;

	AActor* me;

	FVector CenterPos;
	UWorld* world;

};
