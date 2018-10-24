// Fill out your copyright notice in the Description page of Project Settings.

#include "RadarComponent.h"


// Sets default values for this component's properties
URadarComponent::URadarComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URadarComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	InitializeRadar(nullptr, nullptr);

	UE_LOG(LogTemp, Warning, TEXT("HHIIII"))
}


// Called every frame
void URadarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}


void URadarComponent::InitializeRadar(AActor* OwnerPlayer, class UCameraComponent* MainCamera)
{
	PlayerRef = OwnerPlayer;
	CameraRef = MainCamera;

	if (!MainWidgetInstance)
	{
		MainWidgetInstance = CreateWidget<UMainWidget>(GetWorld(), MainWidget);
		MainWidgetInstance->AddToViewport();
	}
}

