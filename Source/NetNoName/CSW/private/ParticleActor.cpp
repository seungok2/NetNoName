// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"
#include "Components\SphereComponent.h"
#include "TimerManager.h"
#include "Kismet\GameplayStatics.h"
#include "Sound/SoundCue.h"
#include "Sound/SoundWave.h"
#include "Components/AudioComponent.h"


// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("sphereColl"));
	SetRootComponent(sphereCol);
	sphereCol->InitSphereRadius(size);
	//sphereCol->SetCollisionProfileName(TEXT("BlockAll"));

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->bAutoActivate = false; // 자동 재생 비활성화

	SetReplicates(true);
}

// Called when the game starts or when spawned
void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
	sphereCol->InitSphereRadius(size);


	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AParticleActor::DestroyActor, DestroyTime, false);

	if (effectSound)
	{
		AudioComponent->SetSound(effectSound);
		AudioComponent->Play();
	}
	else if (effectSoundWave)
	{
		AudioComponent->SetSound(effectSoundWave);
		AudioComponent->Play();
	}
}

// Called every frame
void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleActor::DestroyActor()
{
	if (effectSound)
	{
		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
		}
	}
	else if (effectSoundWave)
	{
		if (AudioComponent && AudioComponent->IsPlaying())
		{
			AudioComponent->Stop();
		}
	}

	if (IsValid(this))
	{
		Destroy();
	}
}



