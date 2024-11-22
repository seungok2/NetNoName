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
private:

protected:
	virtual void Action_MBLeft(const FInputActionValue& Value) override;
	virtual void Action_MBRight(const FInputActionValue& Value) override;
	virtual void Action_Q(const FInputActionValue& Value) override;
	virtual void Action_E(const FInputActionValue& Value) override;
	virtual void Action_R(const FInputActionValue& Value) override;

	virtual void BeginPlay() override;
public:
	APlayer_Revenant();
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	UFUNCTION(BlueprintCallable)
	void ComboAttackSave();
};
