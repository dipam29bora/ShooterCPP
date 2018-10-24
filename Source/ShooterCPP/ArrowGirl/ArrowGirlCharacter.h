// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ArrowGirlCharacter.generated.h"


UCLASS(config=Game)
class AArrowGirlCharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* DiveAnimMontage;
	
	UPROPERTY(EditAnywhere)
	class UAnimMontage* jf_DodgeMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* jb_DodgeMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* jl_DodgeMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* jr_DodgeMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* RecoilAndReloadMontage;

	class UAnimInstance* AnimBPRef;
public:
	AArrowGirlCharacter();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	UPROPERTY(BlueprintReadWrite)
	bool IsAiming = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Aim Crosshair")
	TSubclassOf<class UUserWidget> SelectCroshairWidget;
	//class UUserWidget SelectCroshairWidget;

	UPROPERTY()
	class UUserWidget* WidgetInstacne;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float WalkSpeed = 300.f;

	UPROPERTY(EditAnywhere, Category = "Speed")
	float RunSpeed = 600.f;
	
	UPROPERTY(EditAnywhere, Category = "Speed")
	float AimWalkSpeed = 200.f;

	UPROPERTY(EditAnywhere, Category = "Offset")
	float OffsetMultiplier = 1.5f;
	
	UPROPERTY(BlueprintReadWrite)
	bool IsDiving=false;

	UPROPERTY(BlueprintReadWrite)
	float AimDrawAmount = 0.0f;

	UPROPERTY(EditAnywhere)
	class UStaticMeshComponent* BowMesh;

	UPROPERTY(BlueprintReadWrite)
	class ABowActor* currentBow;

	UPROPERTY()
		bool bReadyToNutral = true;
	float WaitTimeToNormal = 7.5f;

	
protected:



	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);


	void TurnAtRate(float Rate);


	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

	//Aiming Button Pressed
	void AimingPressed();
	//Aiming Button Released
	void AimingReleased();

	//Dive is pressed
	void DivePressed();
	//Dive si released
	void DiveReseased();

	//Holding the Right Gamepad Trigger
	void RightTriggerAxis(float value);

	//Right Trigger is Pressed
	void RightTriggerPressed();

	//Right Trigger is Released
	void RightTriggerReleased();

	UFUNCTION(BlueprintPure)
	float LookAtPitch();

	UFUNCTION(BlueprintPure)
	void GetValuesAimation(bool &isAimHolding, float &aimDrawingAmount, bool &ReadyToNutral) const;

	//For Camera Shifting
	float TargetCameraY = 100.f;
	bool isY = false;

	float f_AxisValue = 0.f;
	float r_AxisValue = 0.f;

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	UFUNCTION(BlueprintPure)
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	UFUNCTION(BlueprintPure)
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintImplementableEvent)
		void AdjustCamera();

	UFUNCTION(BlueprintPure)
		float GetDrawingAmount() const;

	//Some Variable Setups
	UFUNCTION(BlueprintCallable)
		void SetIsDriving(bool NewState);

	UFUNCTION(BlueprintCallable)
		void SetCurrentBow(ABowActor* NewBow);

	UFUNCTION(BlueprintPure)
		inline ABowActor* GetCurrentBow() const { return currentBow; }

};

