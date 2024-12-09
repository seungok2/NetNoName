// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "EnemyAnim.h"
#include "NetNoName\RSS\Player_Revenant.h"

#include "CSW_TestMainUI.h"
//#include "Net/UnrealNetwork.h"

#include "ParticleActor.h"
#include "GuidedActor.h"

#include "Components\CapsuleComponent.h"
#include "NetNoName\RSS\P_Revenant_Primay.h"


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

	attackIndex = 0;
	currentMontage = nullptr;
	
	// Replication 설정
	bReplicates = true;


}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	anim = Cast<UEnemyAnim>(this->GetMesh()->GetAnimInstance());
	
	// 스폰, 시작시 start Motion 실행
	//PlayAnimMontage(startMotion);
	mState = EEnemyState::Start;

	isStart = true;
	isDie = false;
	isStun = false;

	enemyMainUI = CreateWidget<UCSW_TestMainUI>(GetWorld(), enemyMainUIFactory);
	if (enemyMainUI)
	{
		enemyMainUI->AddToViewport();
	}

	currentHp = enemyHp;

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &AEnemy::OnHit);

	// LineTrance
	//FTimerHandle CollisionDetectionTimer;

	//GetWorld()->GetTimerManager().SetTimer(CollisionDetectionTimer, this, &AEnemy::PerformCollisionDetection, 0.5f, true);
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

void AEnemy::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AEnemy, mState);
	DOREPLIFETIME(AEnemy, currentHp);


}

void AEnemy::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (currentHp <= 0) return;


	if (OtherActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *OtherActor->GetName());

		AP_Revenant_Primay* bullet = Cast< AP_Revenant_Primay>(OtherActor);

		if (OtherActor == bullet)
		{
			int32 randomDamage = FMath::RandRange(1000, 5000);
			
			TakeDanmage(randomDamage);
		}

	}


	// 충돌 정보를 출력 (디버깅용)
	UE_LOG(LogTemp, Warning, TEXT("Hit Location: %s"), *Hit.ImpactPoint.ToString());
	UE_LOG(LogTemp, Warning, TEXT("Hit Normal: %s"), *Hit.ImpactNormal.ToString());
}


APlayer_Revenant* AEnemy::FindClosestPlayer()
{
	TArray<AActor*> allActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Revenant::StaticClass(), allActors);

	APlayer_Revenant* closestPlayer = nullptr;
	float closestDist = std::numeric_limits<float>::max();

	for (AActor* actor : allActors)
	{
		APlayer_Revenant* player = Cast<APlayer_Revenant>(actor);
	
		if (player)
		{
			float dist = FVector::Distance(player->GetActorLocation(), GetActorLocation());
			if (dist < closestDist)
			{
				closestDist = dist;
				closestPlayer = player;
			}
		}
	}

	return closestPlayer;
}

APlayer_Revenant* AEnemy::FindFarthestPlayer()
{
	TArray<AActor*> allActors;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayer_Revenant::StaticClass(), allActors);

	APlayer_Revenant* farthestPlayer = nullptr;
	float farthestDist = std::numeric_limits<float>::min();
	for (AActor* actor : allActors)
	{
		APlayer_Revenant* player = Cast<APlayer_Revenant>(actor);

		if (player)
		{
			float dist = FVector::Distance(player->GetActorLocation(), GetActorLocation());
			if (dist > farthestDist)
			{
				farthestDist = dist;
				farthestPlayer = player;
			}
		}
	}

	return farthestPlayer;
}

void AEnemy::ChangeState()
{
	if (HasAuthority()) // 서버인지 확인
	{
		
		FString logmsg = UEnum::GetValueAsString(mState);
		/*GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logmsg);*/

		switch (mState)
		{
		case EEnemyState::Start:
			AniState(&isStart, startMotion);
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
			AniState(&isStun, Stun);
			break;
		case EEnemyState::Die:
			AniState(&isDie, Die);
			break;
		default:
			break;
		}
	}
}

void AEnemy::OnRep_ChangeState()
{
	//FString logmsg = UEnum::GetValueAsString(mState);
	//GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, logmsg);

	/*
	switch (mState)
	{
	case EEnemyState::Start:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Enemy state changed to Start"));
		break;
	case EEnemyState::Idle:
		IdleState();
		break;
	case EEnemyState::Move:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Enemy state changed to Move"));
		MoveState();
		break;
	case EEnemyState::Attack:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Enemy state changed to Attack"));
		AttackState();
		break;
	case EEnemyState::Stun:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Enemy state changed to Stun"));
		break;
	case EEnemyState::Die:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Enemy state changed to Die"));
		break;
	default:
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Unknown Enemy State"));
		break;
	}
	*/
	if (isDie == true) return;

	switch (mState)
	{
	case EEnemyState::Start:
		AniState(&isStart, startMotion);
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
		AniState(&isStun, Stun);
		break;
	case EEnemyState::Die:
		AniState(&isDie, Die);
		break;
	default:
		break;
	}
}

void AEnemy::IdleState()
{
	anim->animState = EEnemyState::Idle;

	if (anim->Montage_IsPlaying(currentMontage))
	{
		float playingMontageTime = currentMontage->GetPlayLength();
		currentTime += GetWorld()->DeltaTimeSeconds;
		if (currentTime >= playingMontageTime+idleDelayTime)
		{
			currentTime = 0;

			targetPlayer = FindClosestPlayer();

			float dis = FVector::Distance(GetActorLocation(), targetPlayer->GetActorLocation());

			if (dis >= attackRang && mState == EEnemyState::Idle)
			{
				mState = EEnemyState::Move;
			}
			else
			{
				mState = EEnemyState::Attack;
			}

		}
	}
	else
	{
		currentTime += GetWorld()->DeltaTimeSeconds;
		if (currentTime >= idleDelayTime)
		{
			currentTime = 0;

			targetPlayer = FindClosestPlayer();

			float dis = FVector::Distance(GetActorLocation(), targetPlayer->GetActorLocation());

			if (dis >= attackRang)
			{
				mState = EEnemyState::Move;
			}
			else
			{
				mState = EEnemyState::Attack;
			}
		}
	}
	
}

void AEnemy::MoveState()
{
	//UE_LOG(LogTemp, Warning, TEXT("Move"));
	anim->animState = EEnemyState::Move;

	targetPlayer = FindClosestPlayer();

	if (targetPlayer)
	{
		float dist = FVector::Distance(targetPlayer->GetActorLocation(), GetActorLocation());
		if (dist <= attackRang)
		{
			//UE_LOG(LogTemp, Warning, TEXT("range in target"));
			anim->animState = EEnemyState::Idle;

			mState = EEnemyState::Attack;
		}
		else
		{
			//UE_LOG(LogTemp, Warning, TEXT("Not range in target"));
			FVector direction = (targetPlayer->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			// 방향과 크기(속도 비율)
			AddMovementInput(direction, 2.0f); 
			// 방향 벡터를 회전값으로 변환
			FRotator newRotation = direction.Rotation();
			newRotation.Pitch = 0.0f;
			SetActorRotation(newRotation);
		}
	}
}

void AEnemy::AttackState()
{
	/* TEST

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
		mState = EEnemyState::Move;
		break;
	}

	if (currentMontage != nullptr)
		PlayAnimMontage(currentMontage);

	mState = EEnemyState::Idle;
	*/

	if (HasAuthority())
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
			mState = EEnemyState::Move;
			break;
		}

		if (currentMontage != nullptr)
			Mult_AttackState(currentMontage);
		
	}

}


void AEnemy::AniState(bool* isState, UAnimMontage* playMotion)
{
	if (*isState)
	{
	
		*isState = false;
		Mult_AniState(playMotion);
		UE_LOG(LogTemp, Warning, TEXT("aniState!!!!!"));
	}
	else
		return;
}

void AEnemy::Mult_AniState_Implementation(UAnimMontage* playMotion)
{
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Red, TEXT("Check 1"));
	PlayAnimMontage(playMotion);
}


void AEnemy::OnRep_CurrentHp()
{
	if (enemyMainUI)
	{
		enemyMainUI->UpdateCurrentHp(currentHp, enemyHp);
	
		if (currentHp <= 0)
		{
			isDie = true;
			mState = EEnemyState::Die;
		}
	}
}

void AEnemy::TakeDanmage(int32 Damage)
{
	if (Damage >= CriticalDamage)
	{
		mState = EEnemyState::Stun;
		isStun = true;
	}

	// server
	if (HasAuthority())
	{
		currentHp -= Damage;

		if (currentHp <= 0)
		{
			currentHp = 0;

			//Mult_UpdateHealthAndDeath(currentHp, enemyHp);
		}
		
		OnRep_CurrentHp();
	}
	else
	{
		// client -> server 요청
		Server_TakeDanmage(Damage);
	}
}


void AEnemy::Server_TakeDanmage_Implementation(int32 damage)
{
	TakeDanmage(damage);
}

void AEnemy::Mult_UpdateHealthAndDeath_Implementation(int32 currHp, int32 MaxHp)
{
	if(currentHp <= 0)
	{
		if (anim)
		{
			// 0.25초 블렌드 아웃 시간으로 현재 재생 중인 모든 몽타주 정지
			anim->Montage_Stop(0.25f);
		}

		isDie = true;
		mState = EEnemyState::Die;
	}
}

void AEnemy::Mult_AttackState_Implementation(UAnimMontage* Montage)
{

	if (Montage)
	{
		PlayAnimMontage(Montage);

		mState = EEnemyState::Idle;
		anim->animState = mState;
	}
}

void AEnemy::Client_SpawnEffect_Implementation(const FVector& pos, TSubclassOf<AParticleActor> actor)
{
	GetWorld()->SpawnActor<AParticleActor>(actor, pos, FRotator::ZeroRotator);
}



void AEnemy::Client_RandomLighting_Implementation(const TArray<FVector>& spawnPos, TSubclassOf<AParticleActor> actor)
{
	for (const FVector& pos : spawnPos)
	{
		GetWorld()->SpawnActor<AParticleActor>(actor, pos, FRotator::ZeroRotator);
	}
}


void AEnemy::Client_RandomPourLightingSpawnn_Implementation(const FVector& spawnPos, TSubclassOf<AParticleActor> actor)
{
	GetWorld()->SpawnActor<AParticleActor>(actor, spawnPos, FRotator::ZeroRotator);
}


void AEnemy::Client_RandomHurricaneCircle_Implementation(const TArray<FVector>& spawnPos, TSubclassOf<AParticleActor> actor)
{
	if (HasAuthority())
		return;


	for (const FVector& pos : spawnPos)
	{
		GetWorld()->SpawnActor<AParticleActor>(actor, pos, FRotator::ZeroRotator);
	}

}

void AEnemy::Client_RandomHurricaneActor_Implementation(const TArray<FVector>& spawnPos, TSubclassOf<AGuidedActor> ActorClass)
{
	if (HasAuthority())
		return;
	

	for (const FVector& pos : spawnPos)
	{
		GetWorld()->SpawnActor<AGuidedActor>(ActorClass, pos, FRotator::ZeroRotator);
	}
}
