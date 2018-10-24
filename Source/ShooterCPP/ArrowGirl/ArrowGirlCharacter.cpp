
#include "ArrowGirlCharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "UObject/ConstructorHelpers.h"


//////////////////////////////////////////////////////////////////////////
// AArrowGirlCharacter

AArrowGirlCharacter::AArrowGirlCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->SetRelativeLocation(FVector(0.f,0.f,56.f));

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;

	//Assigning Skeleton Mesh
	const ConstructorHelpers::FObjectFinder<USkeletalMesh> SkelMesh(TEXT("SkeletalMesh'/Game/CharacterAndAnimations/GirlWithBow/Character/WarriorGirl.WarriorGirl'"));
	GetMesh()->SetSkeletalMesh(SkelMesh.Object);
	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, -97.f));
	GetMesh()->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));


	//creating bow mesh
	BowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bow Mesh"));
	const ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMesh(TEXT("StaticMesh'/Game/CharacterAndAnimations/BowAndArrow/Bow.Bow'"));
	BowMesh->SetStaticMesh(StaticMesh.Object);
	BowMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true),TEXT("BowSocket"));

	const ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMon(TEXT("AnimMontage'/Game/CharacterAndAnimations/GirlWithBow/AnimMontage/dive_Montage.dive_Montage'"));
	DiveAnimMontage = AnimMon.Object;

	const ConstructorHelpers::FObjectFinder<UAnimMontage> AnimMon2(TEXT("AnimMontage'/Game/CharacterAndAnimations/GirlWithBow/AnimMontage/RecoilAndReload.RecoilAndReload'"));
	RecoilAndReloadMontage = AnimMon2.Object;
}

//////////////////////////////////////////////////////////////////////////
// Input

void AArrowGirlCharacter::AimingPressed()
{
	IsAiming = true;
	isY = false;
	bUseControllerRotationYaw = true;
	if (!WidgetInstacne->GetIsVisible())
	{
		WidgetInstacne->AddToViewport();
	}

	Cast<UCharacterMovementComponent>(GetCharacterMovement())->MaxWalkSpeed = AimWalkSpeed;

	AdjustCamera();

	bReadyToNutral = false;
	WaitTimeToNormal = 7.5f;
}

void AArrowGirlCharacter::AimingReleased()
{
	IsAiming = false;
	isY = true;
	bUseControllerRotationYaw = false;
	WidgetInstacne->RemoveFromParent();

	Cast<UCharacterMovementComponent>(GetCharacterMovement())->MaxWalkSpeed = RunSpeed;

	StopAnimMontage(RecoilAndReloadMontage);
	IsDiving = false;

	AdjustCamera();
}

//If the Dive Button is pressed
void AArrowGirlCharacter::DivePressed()
{
	if (!IsAiming)
	{
		if (DiveAnimMontage && !IsDiving)
		{
			IsDiving = true;
			PlayAnimMontage(DiveAnimMontage, 1.f, "Default");
		}
	}
	else
	{
		if (f_AxisValue > 0.8f)
		{
			if (jf_DodgeMontage && !IsDiving)
			{
				IsDiving = true;
				PlayAnimMontage(jf_DodgeMontage, 1.f, "Default");
			}
		}
		else if (f_AxisValue < -0.8f)
		{
			if (jb_DodgeMontage && !IsDiving)
			{
				IsDiving = true;
				PlayAnimMontage(jb_DodgeMontage, 1.f, "Default");
			}
		}
		else if (r_AxisValue > 0.8f)
		{
			if (jr_DodgeMontage && !IsDiving)
			{
				IsDiving = true;
				PlayAnimMontage(jr_DodgeMontage, 1.f, "Default");
			}
		}
		else if (r_AxisValue < -0.8f)
		{
			if (jl_DodgeMontage && !IsDiving)
			{
				IsDiving = true;
				PlayAnimMontage(jl_DodgeMontage, 1.f, "Default");
			}
		}
	}
}

//if the Dive button is released
void AArrowGirlCharacter::DiveReseased()
{

}

//if the right trigger is hold
void AArrowGirlCharacter::RightTriggerAxis(float value)
{
	AimDrawAmount = FMath::Lerp(AimDrawAmount, value * 100.f, GetWorld()->GetDeltaSeconds()*1.2f);
}

//if the right trigger is pressed
void AArrowGirlCharacter::RightTriggerPressed()
{

}

//if the right trigger is released
void AArrowGirlCharacter::RightTriggerReleased()
{
	if (RecoilAndReloadMontage && !IsDiving && IsAiming)
	{
		IsDiving = true;
		PlayAnimMontage(RecoilAndReloadMontage, 1.f, "Default");
	}
}

float AArrowGirlCharacter::LookAtPitch()
{
	FRotator DeltaRotation = FRotator::MakeFromEuler(GetControlRotation().Euler() - GetActorRotation().Euler());
	DeltaRotation.Normalize();
	float PitchAngle = DeltaRotation.Pitch * OffsetMultiplier;
	return PitchAngle;
}

void AArrowGirlCharacter::GetValuesAimation(bool &isAimHolding, float &aimDrawingAmount, bool &ReadyToNutral) const
{
	isAimHolding = IsAiming;
	aimDrawingAmount = AimDrawAmount;
	ReadyToNutral = bReadyToNutral;
}

void AArrowGirlCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AArrowGirlCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AArrowGirlCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AArrowGirlCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AArrowGirlCharacter::LookUpAtRate);

	PlayerInputComponent->BindAxis("FireHold", this, &AArrowGirlCharacter::RightTriggerAxis);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &AArrowGirlCharacter::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &AArrowGirlCharacter::TouchStopped);



	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AArrowGirlCharacter::AimingPressed);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AArrowGirlCharacter::AimingReleased);

	PlayerInputComponent->BindAction("Dive", IE_Pressed, this, &AArrowGirlCharacter::DivePressed);
	PlayerInputComponent->BindAction("Dive", IE_Released, this, &AArrowGirlCharacter::DiveReseased);

	PlayerInputComponent->BindAction("Fired", IE_Released, this, &AArrowGirlCharacter::RightTriggerReleased);
	
}

float AArrowGirlCharacter::GetDrawingAmount() const
{
	return AimDrawAmount;
}

void AArrowGirlCharacter::SetIsDriving(bool NewState)
{
	IsDiving = NewState;
}

void AArrowGirlCharacter::SetCurrentBow(ABowActor* NewBow)
{
	currentBow = NewBow;
}

void AArrowGirlCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
		Jump();
}

void AArrowGirlCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
		StopJumping();
}

void AArrowGirlCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AArrowGirlCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void AArrowGirlCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);

		f_AxisValue = Value;
	}
}

void AArrowGirlCharacter::MoveRight(float Value)
{
	if ( (Controller != NULL) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);

		r_AxisValue = Value;
	}
}

void AArrowGirlCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsAiming && !isY)
	{
		FVector relLocation = GetFollowCamera()->GetRelativeTransform().GetLocation();
		relLocation.Y = FMath::Lerp(relLocation.Y, TargetCameraY, DeltaTime*5.f);

		GetFollowCamera()->SetRelativeLocation(relLocation);
		if (relLocation.Y >= 99.9f)
		{
			isY = true;
		}
	}
	else if (!IsAiming && isY)
	{
		FVector relLocation = GetFollowCamera()->GetRelativeTransform().GetLocation();
		relLocation.Y = FMath::Lerp(relLocation.Y, 0.f, DeltaTime*5.f);

		GetFollowCamera()->SetRelativeLocation(relLocation);
		if (relLocation.Y <= 0.9f)
		{
			isY = false;
			GetFollowCamera()->SetRelativeLocation(FVector::ZeroVector);
		}
	}

	//UE_LOG(LogTemp, Warning, TEXT("Pitch : %s"), *FString::SanitizeFloat(LookAtPitch()))

	if (!bReadyToNutral)
	{
		WaitTimeToNormal -= DeltaTime;
		if (WaitTimeToNormal <= 0)
		{
			bReadyToNutral = true;
			WaitTimeToNormal = 7.5f;
			
		}
	}
}

void AArrowGirlCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (SelectCroshairWidget != nullptr)
	{
		WidgetInstacne = CreateWidget<UUserWidget>(GetWorld(), SelectCroshairWidget);

		
	}
	
}
