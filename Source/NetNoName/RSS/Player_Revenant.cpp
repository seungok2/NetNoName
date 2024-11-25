// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Revenant.h"
#include "EnhancedInputComponent.h"
#include "Projectile_Base.h"
#include "Components/CapsuleComponent.h"

APlayer_Revenant::APlayer_Revenant()
{
	SkeletalMeshPaths.AddUnique(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Meshes/Revenant"));
	SkeletalMeshPaths.AddUnique(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Skins/ChronoBoss/Meshes/Revenant_ChronoBoss"));
	SkeletalMeshPaths.AddUnique(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Skins/FrostKing/Meshes/Revenant_FrostKing"));
	SkeletalMeshPaths.AddUnique(TEXT("/Game/ParagonRevenant/Characters/Heroes/Revenant/Skins/RavenQuill/Meshes/Revenant_RavenQuill"));
	SetSkeletalMeshes();
	GetMesh()->SetSkeletalMesh(SkeletalMeshes[0]);
	
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

void APlayer_Revenant::PrimaryAttack(const FInputActionValue& Value)
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
		{
			PlayAnimMontage(AM_PrimaryAttack);

			auto SpawnedActor = GetWorld()->SpawnActor<AProjectile_Base>(Projectile_Primary, FTransform(Calc_AimTransform(FName("FirePosition"), ECC_Visibility)));
			if (SpawnedActor)
			{
				SpawnedActor->SetActorEnableCollision(true);
				UPrimitiveComponent* PrimitiveComponent = Cast<UPrimitiveComponent>(SpawnedActor->GetRootComponent());
				if (PrimitiveComponent)
				{
					PrimitiveComponent->IgnoreActorWhenMoving(this, true);
				}
			}
		}
	}
}

void APlayer_Revenant::Reload(const FInputActionValue& Value)
{
	
}

void APlayer_Revenant::ReloadEnd(const FInputActionValue& Value)
{
	
}

void APlayer_Revenant::Action_Q(const FInputActionValue& Value)
{
	
}

void APlayer_Revenant::Action_E(const FInputActionValue& Value)
{
	
}

void APlayer_Revenant::Action_R(const FInputActionValue& Value)
{
	
}

void APlayer_Revenant::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetSkeletalMesh(SkeletalMeshes[PlayerID % SkeletalMeshes.Num()]);
	if (AM_Entrance)
		PlayAnimMontage(AM_Entrance);
}

void APlayer_Revenant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayer_Revenant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(IA_MBLeft, ETriggerEvent::Started, this, &APlayer_Revenant::PrimaryAttack);
		EnhancedInputComponent->BindAction(IA_MBRight, ETriggerEvent::Started, this, &APlayer_Revenant::Reload);
		EnhancedInputComponent->BindAction(IA_MBRight, ETriggerEvent::Canceled, this, &APlayer_Revenant::ReloadEnd);
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &APlayer_Revenant::Action_Q);
		EnhancedInputComponent->BindAction(IA_E, ETriggerEvent::Started, this, &APlayer_Revenant::Action_E);
		EnhancedInputComponent->BindAction(IA_R, ETriggerEvent::Started, this, &APlayer_Revenant::Action_R);
	}	
}
