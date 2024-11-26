// Fill out your copyright notice in the Description page of Project Settings.


#include "ParticleActor.h"
#include "Components\SphereComponent.h"
#include "TimerManager.h"

// Sets default values
AParticleActor::AParticleActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	sphereCol = CreateDefaultSubobject<USphereComponent>(TEXT("sphereColl"));
	SetRootComponent(sphereCol);
	sphereCol->InitSphereRadius(size);
	//sphereCol->SetCollisionProfileName(TEXT("BlockAll"));


}

// Called when the game starts or when spawned
void AParticleActor::BeginPlay()
{
	Super::BeginPlay();
	
	sphereCol->InitSphereRadius(size);


	FTimerHandle handle;
	GetWorld()->GetTimerManager().SetTimer(handle, this, &AParticleActor::DestroyActor, DestroyTime, false);
	
	sphereCol->OnComponentBeginOverlap.AddDynamic(this, &AParticleActor::OnOverlapBegin);
}

// Called every frame
void AParticleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AParticleActor::DestroyActor()
{
	if (IsValid(this))
	{
		Destroy();
	}
}

void AParticleActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("hit"));
}

