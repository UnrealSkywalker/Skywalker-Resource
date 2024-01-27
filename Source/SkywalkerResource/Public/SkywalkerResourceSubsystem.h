// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SkywalkerResourceSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class SKYWALKERRESOURCE_API USkywalkerResourceSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
		
public:
	UFUNCTION(BlueprintCallable, Category = SkywalkerResource)
		bool LevelTravel(const FString& InURL, bool bAbsolute = false, bool bShouldSkipGameNotify = false);
	
};
