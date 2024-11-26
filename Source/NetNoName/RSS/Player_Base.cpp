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
	FVector(0.0f, 75.0f, 100.0f),  // 캐릭터의 약간 뒤쪽 위에 배치
	FRotator(-10.0f, 0.0f, 0.0f)); // 약간 아래를 바라보도록 회전
	SpringArmComp->TargetArmLength = 550.0f; // 카메라 거리 설정
	SpringArmComp->bUsePawnControlRotation = true; // 캐릭터 회전과 연동
	SpringArmComp->bDoCollisionTest = true;       // 충돌 방지 활성화
	// 카메라 오프셋 설정
	SpringArmComp->SocketOffset = FVector(0.0f, 50.0f, 20.0f); // 약간 오른쪽으로 오프셋 적용
	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->FieldOfView = 105.0f; // FOV를 넓게 설정해 액션과 몰입감을 높임
	CameraComp->bUsePawnControlRotation = false; // 카메라가 독립적으로 회전하지 않도록 설정
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
	FVector FirePosition = GetMesh()->GetSocketLocation(socketName);
	FVector StartLocation = CameraComp->GetComponentLocation();
	FVector EndLocation = CameraComp->GetComponentLocation() + CameraComp->GetForwardVector() * range;
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, traceChannel, params);
	
	if (bHit)
	{
		EndLocation = Hit.ImpactPoint;
	}

	FRotator LookAtRotator = UKismetMathLibrary::FindLookAtRotation(FirePosition, EndLocation);
	// DrawDebugPoint(GetWorld(), EndLocation, 10, FColor::Red, false, 5);
	// DrawDebugLine(GetWorld(), FirePosition, EndLocation, FColor(255, 0, 255), false, 5);
	return UKismetMathLibrary::MakeTransform(FirePosition, LookAtRotator);
}

void APlayer_Base::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(APlayer_Base, PlayerID);
}
