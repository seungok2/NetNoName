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
class UCSW_TestMainUI;

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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// 충돌처리
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	UPROPERTY(ReplicatedUsing = OnRep_ChangeState)
	EEnemyState mState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	int32 enemyHp = 100000;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHp)
	int32 currentHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	TSubclassOf<UCSW_TestMainUI> enemyMainUIFactory;
	class UCSW_TestMainUI* enemyMainUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float idleDelayTime = 6;
	float currentTime = 0;
	int32 attackIndex;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	float CriticalDamage = 5000.0f;

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
	float attackRang = 500.0f;

	// 가까운 Character 찾기 -> APlayer_Revenant->player_Base 
	APlayer_Revenant* FindClosestPlayer();
	APlayer_Revenant* FindFarthestPlayer();


private:
	UAnimMontage* currentMontage;

	APlayer_Revenant* targetPlayer;

	bool isStun = false;

	bool isDie = false;
//	UPROPERTY(Replicated)
	bool isStart = true;

private:

	void ChangeState();
	// OnRep 함수 선언
	UFUNCTION()
	void OnRep_ChangeState();


	void IdleState();
	void MoveState();
	void AttackState();
	void AniState(bool* isState, UAnimMontage* playMotion);

public:
	UFUNCTION(NetMulticast, Reliable)
	void Mult_AniState(UAnimMontage* playMotion);
	
public:
	// 피격 처리
	UFUNCTION()
	void OnRep_CurrentHp();

	void TakeDanmage(int32 damage);

	UFUNCTION(Server, Reliable)
	void Server_TakeDanmage(int32 damage);
	
	UFUNCTION(NetMulticast, Reliable)
	void Mult_UpdateHealthAndDeath(int32 currHp, int32 MaxHp);



	UFUNCTION(NetMulticast, Reliable)
	void Mult_AttackState(UAnimMontage* Montage);

	// DrumMotionEffect
	UFUNCTION(Client, Reliable)
	void Client_SpawnEffect(const FVector& pos, TSubclassOf<AParticleActor> acotr);
	
	// RandomLighting
	UFUNCTION(Client, Reliable)
	void Client_RandomLighting(const TArray<FVector>& spawnPos, TSubclassOf<AParticleActor> actor);


	// RandomPourLighting
	UFUNCTION(Client, Reliable)
	void Client_RandomPourLightingSpawnn(const FVector& spawnPos, TSubclassOf<AParticleActor> actor);


	// RandomHurricane
	UFUNCTION(Client, Reliable)
	void Client_RandomHurricaneCircle(const TArray<FVector>& spawnPos, TSubclassOf<AParticleActor> actor);
	UFUNCTION(Client, Reliable)
	void Client_RandomHurricaneActor(const TArray<FVector>& spawnPos, TSubclassOf<AGuidedActor> ActorClass);

};
