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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bIsAttacking = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bSaveAttack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	int32 AttackCount;
	
public:
	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_Entrance;
	
	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_PrimaryAttack;

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
private:

protected:
	void PrimaryAttack(const FInputActionValue& Value);
	UFUNCTION(Server, Reliable)
	void ServerRPC_PrimaryAttack(FTransform AimTransForm);
	UFUNCTION(NetMulticast, Reliable)
	void BroadCast_PrimaryAttack(FTransform AimTransForm);	
	
	void Reload(const FInputActionValue& Value);
	void ReloadEnd(const FInputActionValue& Value);
	void Action_Q(const FInputActionValue& Value);
	void Action_E(const FInputActionValue& Value);
	void Action_R(const FInputActionValue& Value);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
public:
	APlayer_Revenant();
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	UFUNCTION(BlueprintCallable)
	void ComboAttackSave();
};