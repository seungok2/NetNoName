// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputActionValue.h"
#include "GameFramework/Character.h"
#include "Player_Base.generated.h"

class UInputAction;
UCLASS()
class NETNONAME_API APlayer_Base : public ACharacter
{
	GENERATED_BODY()
	
private:

protected:
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class UCameraComponent* CameraComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	class USpringArmComponent* SpringArmComp;

	// Input
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputMappingContext* InputMappingContext;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input_Move")
	UInputAction* IA_Move;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input_Move")
	UInputAction* IA_Jump;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input_Move")
	UInputAction* IA_Travel;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input_Look")
	UInputAction* IA_Look;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input_Look")
	bool bLookUpInvert = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_MBLeft;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_MBRight;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_Q;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_E;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputAction* IA_R;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Move)
	bool bIsTravel = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bIsAttacking = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	bool bSaveAttack = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Attack)
	int32 AttackCount;
	
public:
	UPROPERTY(EditAnywhere, Blueprintable, Category = Animation)
	UAnimMontage* AM_Attack;

	UPROPERTY(EditAnywhere, Blueprintable, Category = FX)
	UParticleSystemComponent* MuzzleFlash;

	
private:
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Action_Move(const FInputActionValue& Value);
	virtual void Action_Jump(const FInputActionValue& Value);
	virtual void Action_Travel(const FInputActionValue& Value);
	
	virtual void Action_Look(const FInputActionValue& Value);
	
	virtual void Action_MBLeft(const FInputActionValue& Value);
	virtual void Action_MBRight(const FInputActionValue& Value);
	virtual void Action_Q(const FInputActionValue& Value);
	virtual void Action_E(const FInputActionValue& Value);
	virtual void Action_R(const FInputActionValue& Value);
	
public:
	// Sets default values for this character's properties
	APlayer_Base();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	
	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	UFUNCTION(BlueprintCallable)
	void ComboAttackSave();
};

