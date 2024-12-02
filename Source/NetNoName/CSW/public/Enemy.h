// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Start,
	Idle,
	Move,
	Attack,
	Stun,
	Die
};

UENUM(BlueprintType)
enum class EAttackState : uint8
{
	AttackEnd,
	Combo,
	wideArea1,
	wideArea2,
	InstantDeath
};


class UEnemyAnim;
class APlayer_Revenant;

UCLASS()
class NETNONAME_API AEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "State")
	EEnemyState mState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 enemyHp = 100000;
	int32 currentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float idleDelayTime = 6;
	float currentTime = 0;
	int32 attackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TArray<EAttackState> attackPattern;

	// 몽타주
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* startMotion;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* Combo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* wideArea1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* wideArea2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* InstantDeath;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* Stun;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	UAnimMontage* Die;

	UPROPERTY()
	UEnemyAnim* anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float attackRang = 100.0f;

	// 가까운 Character 찾기 -> APlayer_Revenant->player_Base 
	APlayer_Revenant* FindClosestPlayer();
	APlayer_Revenant* FindFarthestPlayer();


private:
	UAnimMontage* currentMontage;

	APlayer_Revenant* targetPlayer;

	bool isStun = false;
	bool isDie = false;
	bool isStart = true;

private:

	void ChangeState();
	void IdleState();
	void MoveState();
	void AttackState();
	void AniState(bool* isState, UAnimMontage* playMotion);

};
