// Fill out your copyright notice in the Description page of Project Settings.

#include "FakeArrow.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine.h"

// Sets default values
AFakeArrow::AFakeArrow()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AFakeArrow::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFakeArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(bIsFired)
		StartFire();
}

bool AFakeArrow::GetIsFired() const
{
	return bIsFired;
}

void AFakeArrow::SetIsFired(bool newState)
{
	if (newState)
	{
		GetWorldTimerManager().SetTimer(resetTimeHandle, this, &AFakeArrow::Reset, 5.85f, false, 5.85f);
	}
	bIsFired = newState;

	EnableEffect(true);
	
}

void AFakeArrow::StartFire()
{
	this->SetActorHiddenInGame(false);
	FVector forward = GetActorForwardVector();
	FVector newPosition = (forward * UKismetMathLibrary::ClampAngle(MaxForce * ForceFector, 15.0f, MaxForce)) + GetActorLocation();
	SetActorLocation(newPosition,false,nullptr,ETeleportType::TeleportPhysics);

	
}

void AFakeArrow::Reset()
{
	UE_LOG(LogTemp, Warning, TEXT("Reset"));
	bIsFired = false;
	GetWorldTimerManager().ClearTimer(resetTimeHandle);
	this->SetActorHiddenInGame(true);
	this->SetActorRelativeLocation(FVector(0.f));

	EnableEffect(false);

}

