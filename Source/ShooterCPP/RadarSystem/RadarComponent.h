// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RadarSystem/UMG/MainWidget.h"
#include "RadarSystem/UMG/RadarWidget.h"
#include "RadarSystem/UMG/DirectionWidget.h"

#include "RadarComponent.generated.h"


USTRUCT(BlueprintType)
struct FDirections
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
		FName DirectionString;

	UPROPERTY(EditAnywhere)
		int32 DirectionAngle;

};



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SHOOTERCPP_API URadarComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	
		AActor* PlayerRef;

		class UCameraComponent* CameraRef;

public:	
	// Sets default values for this component's properties
	URadarComponent();

	UPROPERTY(EditAnywhere, Category = "Configuration")
		TArray<FDirections> Directions;

	UPROPERTY(EditAnywhere, Category = "Configuration")
		float MaxTranslateOnX = 320.f;

	
	
	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		TSubclassOf<UMainWidget> MainWidget;
	
	UPROPERTY()
	UMainWidget* MainWidgetInstance;

	/*UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		UMainWidget* MainWidget;

	UPROPERTY(EditDefaultsOnly, Category = "Widgets")
		UMainWidget* MainWidget;*/

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "RadarSystem")
	virtual	void InitializeRadar(AActor* OwnerPlayer, class UCameraComponent* MainCamera);
	
};
