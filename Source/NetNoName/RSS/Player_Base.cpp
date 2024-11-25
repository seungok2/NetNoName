// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GM_NoName.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayer_Base::APlayer_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(
		FVector(0.0f, 0.0f, 70.0f),
		FRotator(0, 70, 90));
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0.0f, 60.0f, 0.0f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;
}

void APlayer_Base::SetSkeletalMeshes()
{
	for (FString path : SkeletalMeshPaths)
	{
		ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(*path);
		if (InitMesh.Succeeded())
		{
			SkeletalMeshes.AddUnique(InitMesh.Object);
		}
	}
}

void APlayer_Base::SetAnimClass()
{
	ConstructorHelpers::FClassFinder<UAnimInstance> AnimBP(*AnimClassPath);
	if (AnimBP.Succeeded())
	{
		UE_LOG(LogTemp, Display, TEXT("AnimBP.Succeeded"));
		if (AnimBP.Class)
		{
			GetMesh()->SetAnimClass(AnimBP.Class);
		}
	}
}

// Called when the game starts or when spawned
void APlayer_Base::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		AGM_NoName* gameMode = Cast<AGM_NoName>(GetWorld()->GetAuthGameMode());
		gameMode->AddPlayer(this);
		PlayerID = gameMode->Players.Find(this);
	}
	
}

void APlayer_Base::Action_Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.X);
		AddMovementInput(RightDirection, MovementVector.Y);
	}
}

void APlayer_Base::Action_Jump(const FInputActionValue& Value)
{
	Jump();
}

void APlayer_Base::Action_JumpEnd(const FInputActionValue& Value)
{
	StopJumping();
}

void APlayer_Base::Action_Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(bLookUpInvert ? LookAxisVector.Y : -LookAxisVector.Y);
	}
}


// Called every frame
void APlayer_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayer_Base::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if(APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		// Enhanced Input Subsystem 가져오기
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			// 매핑 컨텍스트를 추가하여 입력 활성화
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
	 GetController<APlayerController>()->PlayerCameraManager->ViewPitchMin = -45.0f;
	// GetController<APlayerController>()->PlayerCameraManager->ViewPitchMax = 15.0f;
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Move 액션 바인딩 (축 입력)
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayer_Base::Action_Move);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayer_Base::Action_Jump);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Completed, this, &APlayer_Base::Action_JumpEnd);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayer_Base::Action_Look);
	}
}

FTransform APlayer_Base::Calc_AimTransform(FName socketName, ECollisionChannel traceChannel ,float range)
{
	FHitResult Hit;
	FVector StartLocation = GetMesh()->GetSocketLocation(socketName);
	FVector EndLocation = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * range;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, traceChannel, params);
	
	if (bHit)
	{
		EndLocation = Hit.ImpactPoint;
	}

	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(StartLocation, EndLocation);
	// DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor(255, 0, 255), false, 5);
	// UE_LOG(LogTemp, Warning, TEXT("Rotator : %f, %f, %f"),LookAtRotator.Yaw,LookAtRotator.Pitch,LookAtRotator.Roll);
	return UKismetMathLibrary::MakeTransform(StartLocation, LookAtRotator);
}

void APlayer_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayer_Base, PlayerID);
}
