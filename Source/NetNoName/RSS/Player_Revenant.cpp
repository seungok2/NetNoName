// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Revenant.h"
#include "EnhancedInputComponent.h"
#include "Components/CapsuleComponent.h"

APlayer_Revenant::APlayer_Revenant()
{
	SkeletalMeshPath = TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Skins/RavenQuill/Meshes/Revenant_RavenQuill");
	SetSkeletalMesh();
	
	AnimClassPath = TEXT("/Game/Bluprint/Player/Revenant/ABP_Revenant");
	SetAnimClass();
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-105), FRotator(0,-90,0));
	
	GetCapsuleComponent()->SetCapsuleHalfHeight(105.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);
}

void APlayer_Revenant::ResetCombo()
{
	AttackCount = 0;
	bSaveAttack = false;
	bIsAttacking= false;
}

void APlayer_Revenant::ComboAttackSave()
{
	if (bSaveAttack)
	{
		bSaveAttack = false;
		
	}
}

void APlayer_Revenant::Action_MBLeft(const FInputActionValue& Value)
{
	//Super::Action_MBLeft(Value);

	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		if (AM_PrimaryAttack)
			PlayAnimMontage(AM_PrimaryAttack);
	}
}

void APlayer_Revenant::Action_MBRight(const FInputActionValue& Value)
{
	//Super::Action_MBRight(Value);
}

void APlayer_Revenant::Action_Q(const FInputActionValue& Value)
{
	//Super::Action_Q(Value);
}

void APlayer_Revenant::Action_E(const FInputActionValue& Value)
{
	//Super::Action_E(Value);
}

void APlayer_Revenant::Action_R(const FInputActionValue& Value)
{
	//Super::Action_R(Value);
}

void APlayer_Revenant::BeginPlay()
{
	Super::BeginPlay();

	if (AM_Entrance)
		PlayAnimMontage(AM_Entrance);
}

void APlayer_Revenant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MBLeft, ETriggerEvent::Started, this, &APlayer_Revenant::Action_MBLeft);
		EnhancedInputComponent->BindAction(IA_MBRight, ETriggerEvent::Started, this, &APlayer_Revenant::Action_MBRight);
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &APlayer_Revenant::Action_Q);
		EnhancedInputComponent->BindAction(IA_E, ETriggerEvent::Started, this, &APlayer_Revenant::Action_E);
		EnhancedInputComponent->BindAction(IA_R, ETriggerEvent::Started, this, &APlayer_Revenant::Action_R);
	}	
}
