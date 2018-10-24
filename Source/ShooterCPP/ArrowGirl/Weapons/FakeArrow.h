// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FakeArrow.generated.h"

UCLASS()
class SHOOTERCPP_API AFakeArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFakeArrow();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		bool bIsFired;

	UPROPERTY(BlueprintReadWrite)
		float MaxForce = 35;

	UPROPERTY(BlueprintReadWrite)
		float ForceFector = 0;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	UFUNCTION(BlueprintPure)
		bool GetIsFired() const;

	UFUNCTION(BlueprintCallable)
		void SetIsFired(bool newState);

	UFUNCTION(BlueprintCallable)
		void StartFire();

	UFUNCTION(BlueprintCallable)
		void Reset();

	FTimerHandle resetTimeHandle;

	UFUNCTION(BlueprintImplementableEvent)
		void EnableEffect(bool b);
};
