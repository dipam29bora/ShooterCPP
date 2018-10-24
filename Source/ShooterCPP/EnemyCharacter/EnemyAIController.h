// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"


/**
 * 
 */
UCLASS()
class SHOOTERCPP_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
public:
	AEnemyAIController();
	UPROPERTY(EditAnywhere)
	FVector FocusPoint;
	
private:
	UPROPERTY(Transient)
	class UBehaviorTreeComponent* BTComponent;

	UPROPERTY(Transient)
	class UBlackboardComponent* BBComponent;

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	void SetEnemyToBlackboard();

protected:
	virtual void Possess(APawn* InPawn) override;
};
