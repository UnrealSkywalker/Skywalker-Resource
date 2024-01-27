// Fill out your copyright notice in the Description page of Project Settings.


#include "SkywalkerResourceSubsystem.h"

bool USkywalkerResourceSubsystem::LevelTravel(const FString& InURL, bool bAbsolute, bool bShouldSkipGameNotify)
{
	return GetWorld()->ServerTravel(InURL, bAbsolute, bShouldSkipGameNotify);
}
