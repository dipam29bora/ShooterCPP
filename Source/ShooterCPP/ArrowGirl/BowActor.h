// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BowActor.generated.h"

UCLASS()
class SHOOTERCPP_API ABowActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABowActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		class UStaticMeshComponent* BowMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		int32 maxNoOfBows = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Config")
		TSubclassOf<AActor> Arrow;

	UPROPERTY(BlueprintReadWrite, Category = "Config")
		TArray<AActor*> Arrows;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnPoolingArrows();
	
	UFUNCTION(BlueprintCallable)
		void ReleaseArrow();
};
