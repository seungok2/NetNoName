// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "EnemyAnim.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (GetMesh())
	{
		GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
		// 메시 회전 설정
		GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	}

	mState = EEnemyState::Idle;
	attackIndex = 0;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	anim = Cast<UEnemyAnim>(this->GetMesh()->GetAnimInstance());
	currentMontage = nullptr;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ChangeState();
}

// Called to bind functionality to input
void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::ChangeState()
{
	FString logmsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logmsg);

	switch (mState)
	{
	case EEnemyState::Start:
		StratState();
		break;
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		MoveState();
		break;
	case EEnemyState::Attack:
		AttackState();
		break;
	case EEnemyState::Stun:
		StunState();
		break;
	case EEnemyState::Die:
		DieState();
		break;
	default:
		break;
	}
}

void AEnemy::StratState()
{
}

void AEnemy::IdleState()
{
	//currentTime += GetWorld()->DeltaTimeSeconds;
	//if (currentTime >= idleDelayTime)
	//{
	//	mState = EEnemyState::Attack;

	//	currentTime = 0;

	//	anim->animState = mState;
	//}
	
	if (anim->Montage_IsPlaying(currentMontage))
	{
		float playingMontageTime = currentMontage->GetPlayLength();
		currentTime += GetWorld()->DeltaTimeSeconds;
		if (currentTime >= playingMontageTime+idleDelayTime)
		{
			mState = EEnemyState::Attack;
			
			currentTime = 0;
			
			anim->animState = mState;
		}
	}
	else
	{
		currentTime += GetWorld()->DeltaTimeSeconds;
		if (currentTime >= idleDelayTime)
		{
			mState = EEnemyState::Attack;

			currentTime = 0;

			anim->animState = mState;
		}
	}
	
}

void AEnemy::MoveState()
{
}

void AEnemy::AttackState()
{
	switch (attackPattern[attackIndex])
	{
	case EAttackState::Combo:
		// 몽타주 재생
		currentMontage = Combo;
		
		// 다음 패턴
		UE_LOG(LogTemp, Warning, TEXT("Combo"));
		attackIndex++;
		break;
	case EAttackState::wideArea1:
		currentMontage = wideArea1;
		
		UE_LOG(LogTemp, Warning, TEXT("wideArea1"));
		attackIndex++;
		break;
	case EAttackState::wideArea2:
		currentMontage = wideArea2;
		
		UE_LOG(LogTemp, Warning, TEXT("wideArea2"));
		attackIndex++;
		break;
	case EAttackState::InstantDeath:
		currentMontage = InstantDeath;
		
		UE_LOG(LogTemp, Warning, TEXT("InstantDeath"));
		attackIndex++;
		break;
	default:

		currentMontage = nullptr;
		attackIndex = 0;
		break;
	}

	if (currentMontage != nullptr)
		PlayAnimMontage(currentMontage);


	mState = EEnemyState::Idle;
}

void AEnemy::StunState()
{
}

void AEnemy::DieState()
{
}
