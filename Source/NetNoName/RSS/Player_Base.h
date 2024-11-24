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

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path")
	TArray<FString> SkeletalMeshPaths;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "SkeletalMesh")
	TArray<USkeletalMesh*> SkeletalMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Path")
	FString AnimClassPath;

	
private:
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Action_Move(const FInputActionValue& Value);
	virtual void Action_Jump(const FInputActionValue& Value);
	virtual void Action_JumpEnd(const FInputActionValue& Value);
	virtual void Action_Look(const FInputActionValue& Value);
public:
	// Sets default values for this character's properties
	APlayer_Base();
	void SetSkeletalMeshes();
	void SetAnimClass();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	FTransform Calc_AimTransform(FName socketName, ECollisionChannel traceChannel, float range = 10000);
};

