// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Player_Base.h"
#include "Player_Revenant.generated.h"

class UInputAction;
/**
 * 
 */
UCLASS()
class NETNONAME_API APlayer_Revenant : public APlayer_Base
{
	GENERATED_BODY()
private:

protected:
	UPROPERTY(ReplicatedUsing=On_ChangeHP ,EditAnywhere, BlueprintReadOnly, Category = PlayerStatus)
	float PlayerHP_Max = 1000;
	UPROPERTY(ReplicatedUsing=On_ChangeHP ,EditAnywhere, BlueprintReadOnly, Category = PlayerStatus)
	float PlayerHP_Current = 1000;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadOnly, Category = PlayerStatus)
	bool bIsDead = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Hit)
	float HitAngle = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Hit)
	bool bHitAnimationActive = false;
	
	UPROPERTY(Replicated ,EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bIsCombatMode = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bIsAttacking = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bSaveAttack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	int32 AttackCount;
	
public:
	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimInstance* AnimInstance;
	
	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_Entrance;
	
	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_PrimaryAttack;

	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_Reload;

	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_HitReact;

	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_Death;
	
	UPROPERTY(EditAnywhere, Blueprintable, Category = Projectile)
	TSubclassOf<class AProjectile_Base> Projectile_Primary;

	UPROPERTY(VisibleAnywhere, Category = Projectile)
	int8 MaxPrimaryProjectileCount = 4;

	UPROPERTY(EditAnywhere, Blueprintable, Category = Projectile)
	int8 CurrentPrimaryProjectileCount = 4;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUserWidget> CrossHairWidgetClass;
	UPROPERTY()
	class UUserWidget* CrossHairWidget;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class UUI_PlayerInfo> PlayerInfoWidgetClass;
	UPROPERTY()
	class UUI_PlayerInfo* PlayerInfoWidget;
private:

protected:
	void PrimaryAttack();
	UFUNCTION(Server, Reliable)
	void ServerRPC_PrimaryAttack(FTransform AimTransForm);
	UFUNCTION(NetMulticast, Reliable)
	void BroadCast_PrimaryAttack(FTransform AimTransForm);	
	
	void Reload();
	UFUNCTION(Server, Reliable)
	void ServerRPC_Reload();
	UFUNCTION(NetMulticast, Reliable)
	void BroadCast_Reload();
	
	void ChangeCombatMode();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	
	UFUNCTION(NetMulticast, Reliable)
	void BroadCast_TakeDamage(float Angle);
	UFUNCTION(NetMulticast, Reliable)
	void BroadCast_Die(float Angle);

	UFUNCTION()
	void On_ChangeHP();
public:
	APlayer_Revenant();
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	UFUNCTION(BlueprintCallable)
	void ComboAttackSave();
	UFUNCTION(BlueprintCallable)
	void Die();
};