// Fill out your copyright notice in the Description page of Project Settings.


#include "Player_Base.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlayer_Base::APlayer_Base()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, 70.0f),
		FRotator(0, 70, 90));
	SpringArmComp->TargetArmLength = 300;
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->SocketOffset = FVector(0.0f, 60.0f, 0.0f);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);
	CameraComp->bUsePawnControlRotation = false;


	GetCapsuleComponent()->SetCapsuleHalfHeight(95.0f);
	ConstructorHelpers::FObjectFinder<USkeletalMesh> InitMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonWraith/Characters/Heroes/Wraith/Skins/ODGreen/Meshes/Wraith_ODGreen.Wraith_ODGreen'"));
	if (InitMesh.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(InitMesh.Object);
		GetMesh()->SetRelativeLocationAndRotation(FVector(0,0,-95),FRotator(0,-90,0));
	}	
}

// Called when the game starts or when spawned
void APlayer_Base::BeginPlay()
{
	Super::BeginPlay();
	
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

void APlayer_Base::Action_Travel(const FInputActionValue& Value)
{
	bIsTravel = !bIsTravel;
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

void APlayer_Base::Action_MBLeft(const FInputActionValue& Value)
{
	
}

void APlayer_Base::Action_MBRight(const FInputActionValue& Value)
{
	
}

void APlayer_Base::Action_Q(const FInputActionValue& Value)
{
	
}

void APlayer_Base::Action_E(const FInputActionValue& Value)
{
	
}

void APlayer_Base::Action_R(const FInputActionValue& Value)
{

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
	GetController<APlayerController>()->PlayerCameraManager->ViewPitchMax = 15.0f;

	// Enhanced Input Component 설정
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		// Move 액션 바인딩 (축 입력)
		EnhancedInputComponent->BindAction(IA_Move, ETriggerEvent::Triggered, this, &APlayer_Base::Action_Move);
		EnhancedInputComponent->BindAction(IA_Jump, ETriggerEvent::Started, this, &APlayer_Base::Action_Jump);
		EnhancedInputComponent->BindAction(IA_Travel, ETriggerEvent::Started, this, &APlayer_Base::Action_Travel);
		EnhancedInputComponent->BindAction(IA_Look, ETriggerEvent::Triggered, this, &APlayer_Base::Action_Look);
		EnhancedInputComponent->BindAction(IA_MBLeft, ETriggerEvent::Started, this, &APlayer_Base::Action_MBLeft);
		EnhancedInputComponent->BindAction(IA_MBRight, ETriggerEvent::Started, this, &APlayer_Base::Action_MBRight);
		EnhancedInputComponent->BindAction(IA_Q, ETriggerEvent::Started, this, &APlayer_Base::Action_Q);
		EnhancedInputComponent->BindAction(IA_E, ETriggerEvent::Started, this, &APlayer_Base::Action_E);
		EnhancedInputComponent->BindAction(IA_R, ETriggerEvent::Started, this, &APlayer_Base::Action_R);
	}
}

