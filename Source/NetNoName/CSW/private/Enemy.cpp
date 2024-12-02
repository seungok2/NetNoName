// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Kismet\GameplayStatics.h"
#include "EnemyAnim.h"
#include "NetNoName\RSS\Player_Revenant.h"

#include "CSW_TestMainUI.h"

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
	
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
	anim = Cast<UEnemyAnim>(this->GetMesh()->GetAnimInstance());
	
	// 스폰, 시작시 start Motion 실행
	PlayAnimMontage(startMotion);
	isDie = false;


	enemyMainUI = CreateWidget<UCSW_TestMainUI>(GetWorld(), enemyMainUIFactory);
	if (enemyMainUI)
	{
		enemyMainUI->AddToViewport();
	}

	currentHp = enemyHp;
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
	FString logmsg = UEnum::GetValueAsString(mState);
	GEngine->AddOnScreenDebugMessage(0, 1, FColor::Cyan, logmsg);

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
			AddMovementInput(direction, 1.0f); 
			// 방향 벡터를 회전값으로 변환
			FRotator newRotation = direction.Rotation();
			newRotation.Pitch = 0.0f;
			SetActorRotation(newRotation);
		}
	}
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


void AEnemy::AniState(bool* isState, UAnimMontage* playMotion)
{
	if (*isState == true)
	{
		*isState = false;
		PlayAnimMontage(playMotion);
		UE_LOG(LogTemp, Warning, TEXT("aniState"));
	}
	else
		return;
}

void AEnemy::Danmage(int32 Damage)
{
	currentHp -= Damage;

	if (currentHp <= 0)
	{
		mState = EEnemyState::Die;
		isDie = true;
	}

	enemyMainUI->UpdateCurrentHp(currentHp, enemyHp);
	
}
