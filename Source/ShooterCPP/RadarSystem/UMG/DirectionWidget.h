// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DirectionWidget.generated.h"

/**
 * 
 */
UCLASS()
class SHOOTERCPP_API UDirectionWidget : public UUserWidget
{
	GENERATED_BODY()
	
		//UFUNCTION(BlueprintImplementableEvent, BlueprintCosmetic, Category = "User Interface")
	void PreConstruct_Implementation(bool IsDesignTime);
	
	
};
