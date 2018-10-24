// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "EnemyCharacter/EnemyCharacter.h"

AEnemyAIController::AEnemyAIController()
{
	BTComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree Component"));
	BBComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("Blackboard Component"));
}

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	SetEnemyToBlackboard();
}

void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAIController::SetEnemyToBlackboard()
{
	FocusPoint = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();	

	BBComponent->SetValueAsVector("FocusPoint", FocusPoint);
}

void AEnemyAIController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	auto character = Cast<AEnemyCharacter>(InPawn);
	if (character && character->EnemyBehaviorTree)
	{
		BBComponent->InitializeBlackboard(*(character->EnemyBehaviorTree->BlackboardAsset));
		BTComponent->StartTree(*(character->EnemyBehaviorTree));
	}
}
