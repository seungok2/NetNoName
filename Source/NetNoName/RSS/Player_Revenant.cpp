// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Revenant.h"
#include "EnhancedInputComponent.h"
#include "Projectile_Base.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"

APlayer_Revenant::APlayer_Revenant()
{
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

void APlayer_Revenant::PrimaryAttack()
{
	//Super::Action_MBLeft(Value);
	if (CurrentPrimaryProjectileCount <= 0)
	{
		Reload();
		return;
	}
	
	if (bIsAttacking)
	{
		bSaveAttack = true;
	}
	else
	{
		bIsAttacking = true;
		FTransform AimTransForm = FTransform(Calc_AimTransform(FName("FirePosition"), ECC_Visibility));
		CurrentPrimaryProjectileCount--;
		ServerRPC_PrimaryAttack(AimTransForm);
	}
}

void APlayer_Revenant::ServerRPC_PrimaryAttack_Implementation(FTransform AimTransForm)
{
	BroadCast_PrimaryAttack(AimTransForm);
}

void APlayer_Revenant::BroadCast_PrimaryAttack_Implementation(FTransform AimTransForm)
{
	if (AM_PrimaryAttack)
	{
		PlayAnimMontage(AM_PrimaryAttack);
	}
	auto SpawnedActor = GetWorld()->SpawnActor<AProjectile_Base>(Projectile_Primary, AimTransForm);
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

void APlayer_Revenant::Reload()
{
	UE_LOG(LogTemp, Warning, TEXT("Reload"));
	if (bIsAttacking) return;
	if (CurrentPrimaryProjectileCount == MaxPrimaryProjectileCount) return;

	CurrentPrimaryProjectileCount = MaxPrimaryProjectileCount;
	bIsAttacking = true;
	ServerRPC_Reload();
}

void APlayer_Revenant::ServerRPC_Reload_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ServerRPC_Reload_Implementation"));
	BroadCast_Reload();
}

void APlayer_Revenant::BroadCast_Reload_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("BroadCast_Reload_Implementation"));
	if (AM_Reload)
	{
		UE_LOG(LogTemp, Warning, TEXT("AM_Reload"));
		PlayAnimMontage(AM_Reload);
	}
}

void APlayer_Revenant::Action_Q()
{
	
}

void APlayer_Revenant::Action_E()
{
	
}

void APlayer_Revenant::Action_R()
{
	
}

void APlayer_Revenant::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetSkeletalMesh(Skins[PlayerID % Skins.Num()]);
	if (AM_Entrance)
		PlayAnimMontage(AM_Entrance);

	if(IsLocallyControlled() == false) return;

	CrossHairWidget = Cast<UUserWidget>( CreateWidget(GetWorld(), CrossHairWidgetClass) );
	CrossHairWidget->AddToViewport();
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
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &APlayer_Revenant::Action_Q);
		EnhancedInputComponent->BindAction(IA_E, ETriggerEvent::Started, this, &APlayer_Revenant::Action_E);
		EnhancedInputComponent->BindAction(IA_R, ETriggerEvent::Started, this, &APlayer_Revenant::Action_R);
	}	
}
