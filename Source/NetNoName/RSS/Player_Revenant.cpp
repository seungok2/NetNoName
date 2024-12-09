// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Revenant.h"
#include "EnhancedInputComponent.h"
#include "ParticleActor.h"
#include "Projectile_Base.h"
#include "UI_PlayerInfo.h"
#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

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

void APlayer_Revenant::Die()
{
	UE_LOG(LogTemp, Warning, TEXT("AnimNotify_Die : %s"), *GetActorNameOrLabel());
	GetMesh()->SetVisibility(false);
	
}

void APlayer_Revenant::PrimaryAttack()
{
	if (bIsDead) return;
	
	if (!bIsCombatMode)
	{
		bIsCombatMode = true;
	}
	
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
		AnimInstance->Montage_Play(AM_PrimaryAttack);
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
	if (bIsDead) return;
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
		AnimInstance->Montage_Play(AM_Reload);
	}
}

void APlayer_Revenant::ChangeCombatMode()
{
	if (bIsDead) return;
	bIsCombatMode = !bIsCombatMode;
}

void APlayer_Revenant::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APlayer_Revenant,PlayerHP_Max);
	DOREPLIFETIME(APlayer_Revenant,PlayerHP_Current);
	DOREPLIFETIME(APlayer_Revenant,bIsCombatMode);
	DOREPLIFETIME(APlayer_Revenant,bIsDead);
}

void APlayer_Revenant::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetSkeletalMesh(Skins[PlayerID % Skins.Num()]);
	AnimInstance = GetMesh()->GetAnimInstance();
	
	if (AM_Entrance)
		AnimInstance->Montage_Play(AM_Entrance);

	if(IsLocallyControlled())
	{
		CrossHairWidget = Cast<UUserWidget>( CreateWidget(GetWorld(), CrossHairWidgetClass) );
		CrossHairWidget->AddToViewport();

		PlayerInfoWidget = Cast<UUI_PlayerInfo>( CreateWidget(GetWorld(), PlayerInfoWidgetClass) );
		PlayerInfoWidget->AddToViewport();
		PlayerInfoWidget->SetHP(PlayerHP_Current, PlayerHP_Max);		
	}
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
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &APlayer_Revenant::ChangeCombatMode);
		EnhancedInputComponent->BindAction(IA_E, ETriggerEvent::Started, this, &APlayer_Revenant::ChangeCombatMode);
		EnhancedInputComponent->BindAction(IA_R, ETriggerEvent::Started, this, &APlayer_Revenant::Reload);
	}	
}

float APlayer_Revenant::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	if (HasAuthority())
	{
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage"));
		float ImpactAngle = 0;
		
		if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
		{
			FPointDamageEvent* PointDamageEvent = (FPointDamageEvent*)&DamageEvent;
			FVector ImpactPoint = PointDamageEvent->HitInfo.ImpactPoint; // 충돌 포인트 (월드 좌표)
			FVector ActorLocation = GetActorLocation(); // 액터의 위치
			FVector ActorForward = GetActorForwardVector(); // 액터의 전방 벡터
		
			// 충돌 방향 벡터 계산 (정규화)
			FVector ImpactDirection = (ImpactPoint - ActorLocation);
			ImpactDirection.Z = 0; 
			ImpactDirection = ImpactDirection.GetSafeNormal();

			// 액터 정면 벡터
			FVector ForwardVector = ActorForward;
			ForwardVector.Z = 0; 
			ForwardVector = ForwardVector.GetSafeNormal();

			// 내적 계산
			float DotProduct = FVector::DotProduct(ForwardVector, ImpactDirection);
			
			ImpactAngle = FMath::Acos(DotProduct);

			ImpactAngle = FMath::RadiansToDegrees(ImpactAngle);

			// 외적 계산
			FVector CrossProduct = FVector::CrossProduct(ForwardVector, ImpactDirection);

			// Z축 확인 (양수: 오른쪽, 음수: 왼쪽)
			float Side = FVector::DotProduct(CrossProduct, GetActorUpVector());

			ImpactAngle *= (Side>=0 ? 1 : -1 );
		}
		
		PlayerHP_Current -= DamageAmount;
		On_ChangeHP();
		
		UE_LOG(LogTemp, Warning, TEXT("TakeDamage [%s] : %f, ImpactAngle : %f"), *GetActorNameOrLabel(), PlayerHP_Current, ImpactAngle);
		
		if(PlayerHP_Current <= 0 && bIsDead == false)
		{
			bIsDead = true;

			// 움직이지 못하게 하자.
			GetCharacterMovement()->DisableMovement();
			BroadCast_Die(ImpactAngle);
		}
		else
		{
			BroadCast_TakeDamage(ImpactAngle);
		}
	}
	return Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
}

void APlayer_Revenant::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (HasAuthority())
	{
		if (OtherActor->IsA(AParticleActor::StaticClass()))
		{
			//UE_LOG(LogTemp, Warning, TEXT("APlayer_Revenant : NotifyActorBeginOverlap"));
			float ImpactAngle = 0;
			int32 DamageAmount = FMath::RandRange( 100,500 );
		
			PlayerHP_Current -= DamageAmount;
			On_ChangeHP();
		
			UE_LOG(LogTemp, Warning, TEXT("NotifyActorBeginOverlap [%s] : %f"), *GetActorNameOrLabel(), PlayerHP_Current);
		
			if(PlayerHP_Current <= 0 && bIsDead == false)
			{
				bIsDead = true;

				// 움직이지 못하게 하자.
				GetCharacterMovement()->DisableMovement();
				BroadCast_Die(ImpactAngle);
			}
			else
			{
				BroadCast_TakeDamage(ImpactAngle);
			}
		}
	}	
}

void APlayer_Revenant::BroadCast_TakeDamage_Implementation(float ImpactAngle)
{
	UE_LOG(LogTemp, Warning, TEXT("BroadCast_TakeDamage [ImpactAngle : %f]"),ImpactAngle);
	HitAngle = ImpactAngle;

	//애니메이션 실행?!
	bHitAnimationActive = true;
}

void APlayer_Revenant::BroadCast_Die_Implementation(float ImpactAngle)
{
	UE_LOG(LogTemp, Warning, TEXT("BroadCast_Die [ImpactAngle : %f]"),ImpactAngle);
	bool bIsForward = (FMath::Abs(ImpactAngle) < 90);
	FName Section = bIsForward?FName("Forward"):FName("Backward");
	//UE_LOG(LogTemp, Warning, TEXT("BroadCast_TakeDamage bIsDead : %s"), *Section.ToString());
	float returnValue = AnimInstance->Montage_Play(AM_Death, 1);

	if (returnValue > 0.0f)
	{
		AnimInstance->Montage_JumpToSection(Section, AM_Death);
	}
}

void APlayer_Revenant::On_ChangeHP()
{
	if(IsLocallyControlled())
	{
		PlayerInfoWidget->SetHP(PlayerHP_Current, PlayerHP_Max);		
	}
}