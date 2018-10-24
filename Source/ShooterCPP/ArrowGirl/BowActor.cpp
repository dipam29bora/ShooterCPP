// Fill out your copyright notice in the Description page of Project Settings.

#include "BowActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Weapons/FakeArrow.h"


// Sets default values
ABowActor::ABowActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	if (!RootComponent)
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	BowMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BowMesh"));
	BowMesh->SetupAttachment(RootComponent);

	//static ConstructorHelpers::FObjectFinder<UBlueprint> item(TEXT("'Blueprint'/Game/Blueprints/Weapons/Arrow_Obj.Arrow_Obj'"));
	//Arrow = (UClass*)item.Object->RegenerateClass(Arrow);

}

// Called when the game starts or when spawned
void ABowActor::BeginPlay()
{
	Super::BeginPlay();

	if (Arrow)
	{
		SpawnPoolingArrows();
	}
	
}

// Called every frame
void ABowActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABowActor::SpawnPoolingArrows()
{


	FTransform transform = BowMesh->GetSocketTransform("ArrowSocket", ERelativeTransformSpace::RTS_World);
	
	
	FActorSpawnParameters spawnInfo;
	
	for (int i=0; i<maxNoOfBows; i++)
	{
		 AActor* C = GetWorld()->SpawnActor<AActor>((UClass*)Arrow, transform, spawnInfo);
		 C->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
		 C->SetActorRelativeLocation(FVector(0.0f));
		 Arrows.Add(C);
	}

	
}

void ABowActor::ReleaseArrow()
{
	for (int i=0; i<Arrows.Num(); i++)
	{
		auto arrow = Cast<AFakeArrow>(Arrows[i]);
		if (arrow && !arrow->bIsFired)
		{
			arrow->SetIsFired(true);
			break;
		}
	}
}

